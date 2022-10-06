#include "telegrambotconfiguration.h"

#include <algorithm>
#include "configurationtools.h"
#include "stringtools.h"

using namespace std;

const wstring TelegramBotConfiguration::BotTokenProperty = L"BotToken";
const wstring TelegramBotConfiguration::AuthorizedUserTokenProperty = L"AuthorizedUserToken";
const wstring TelegramBotConfiguration::AuthorizedUserChatIdProperty = L"AuthorizedUserChatId";

std::wstring TelegramBotConfiguration::GetName()
{
   return L"TelegramBot";
}

TelegramBotData* TelegramBotConfiguration::Load(ConfigurationObject* confObject,
                                                vector<wstring>& errorMessages)
{
   TelegramBotData* data = new TelegramBotData();

   data->botToken = StringTools::UnicodeToUtf8(confObject->GetProperty(BotTokenProperty));
   data->authorizedUserToken = StringTools::UnicodeToUtf8(confObject->GetProperty(AuthorizedUserTokenProperty));
   data->authorizedUserChatId = StringTools::UnicodeToUtf8(confObject->GetProperty(AuthorizedUserChatIdProperty));

   const vector<wstring> handledProperties = {BotTokenProperty, AuthorizedUserTokenProperty,
                                              AuthorizedUserChatIdProperty};
   CheckUnhandledProperties(confObject, handledProperties, errorMessages);
   return data;
}

void TelegramBotConfiguration::SaveToFile(const TelegramBotData& data,
                                          std::wofstream& fileStream)
{
   ConfigurationTools::SaveValueToFile(fileStream, BotTokenProperty, StringTools::Utf8ToUnicode(data.botToken));
   ConfigurationTools::SaveValueToFile(fileStream, AuthorizedUserTokenProperty, StringTools::Utf8ToUnicode(data.authorizedUserToken));
   ConfigurationTools::SaveValueToFile(fileStream, AuthorizedUserChatIdProperty, StringTools::Utf8ToUnicode(data.authorizedUserChatId));
}

void TelegramBotConfiguration::CheckUnhandledProperties(ConfigurationObject* confObject,
                                                        const std::vector<wstring>& handledProperties,
                                                        std::vector<wstring>& errorMessages)
{
   map<wstring, wstring>::iterator itProp = confObject->BeginProperties();
   map<wstring, wstring>::iterator endProp = confObject->EndProperties();
   for (; itProp != endProp; itProp++)
   {
       pair<wstring, wstring> currentProp = *itProp;
       auto finder = [&currentProp](const wstring& value)
       {
         return value == currentProp.first;
       };
       vector<wstring>::const_iterator it = std::find_if(
                handledProperties.begin(), handledProperties.end(), finder);
       if (it == handledProperties.end())
          errorMessages.push_back(ConfigurationTools::CreateUnhandledProperty(currentProp.first));
   }
}

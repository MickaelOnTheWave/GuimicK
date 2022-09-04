#include "telegrambotconfiguration.h"

#include <algorithm>
#include "configurationtools.h"
#include "stringtools.h"

using namespace std;

std::wstring TelegramBotConfiguration::GetName()
{
   return L"TelegramBot";
}

TelegramBotData* TelegramBotConfiguration::Load(ConfigurationObject* confObject,
                                                vector<wstring>& errorMessages)
{
   TelegramBotData* data = new TelegramBotData();

   data->botToken = StringTools::UnicodeToUtf8(confObject->GetProperty(L"BotToken"));
   data->authorizedUserToken = StringTools::UnicodeToUtf8(confObject->GetProperty(L"AuthorizedUserToken"));
   data->authorizedUserChatId = StringTools::UnicodeToUtf8(confObject->GetProperty(L"AuthorizedUserChatId"));

   const vector<wstring> handledProperties = {L"BotToken", L"AuthorizedUserToken",
                                              L"AuthorizedUserChatId"};
   CheckUnhandledProperties(confObject, handledProperties, errorMessages);
   return data;
}

void TelegramBotConfiguration::SaveToFile(const TelegramBotData& data,
                                          std::wofstream& fileStream)
{
   ConfigurationTools::SaveValueToFile(fileStream, L"BotToken", StringTools::Utf8ToUnicode(data.botToken));
   ConfigurationTools::SaveValueToFile(fileStream, L"AuthorizedUserToken", StringTools::Utf8ToUnicode(data.authorizedUserToken));
   ConfigurationTools::SaveValueToFile(fileStream, L"AuthorizedUserChatId", StringTools::Utf8ToUnicode(data.authorizedUserChatId));
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

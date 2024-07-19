#include "emailaccountdataconfiguration.h"

#include <map>
#include "configurationtools.h"
#include "stringtools.h"

using namespace std;

EmailAccountDataConfiguration::EmailAccountDataConfiguration(
        EmailAccountData& _dataObject
    ) : dataObject(_dataObject)
{
}

wstring EmailAccountDataConfiguration::GetName() const
{
   return L"EmailAccount";
}

void EmailAccountDataConfiguration::Load(ConfigurationObject* confObject,
                                                          vector<wstring>& errorMessages)
{
    map<wstring, wstring>::iterator itProp = confObject->BeginProperties();
    map<wstring, wstring>::iterator endProp = confObject->EndProperties();
    for (; itProp != endProp; itProp++)
    {
        pair<wstring, wstring> currentProp = *itProp;
        LoadProperty(currentProp, errorMessages);
    }
}

void EmailAccountDataConfiguration::SaveToFile(const EmailAccountData& data,
                                               wofstream& fileStream)
{
    if (data.IsValid())
    {
       ConfigurationTools::SaveValueToFile(fileStream, L"Email", data.GetAddress());
       ConfigurationTools::SaveValueToFile(fileStream, L"Password", data.GetPassword());
       ConfigurationTools::SaveValueToFile(fileStream, L"SmtpAddress", data.GetSmtpServer());
       ConfigurationTools::SaveValueToFile(fileStream, L"SmtpPort", data.GetSmtpPort());
       ConfigurationTools::SaveValueToFile(fileStream, L"UseSSL", data.GetUseSsl());
    }
}

void EmailAccountDataConfiguration::LoadProperty(
      const pair<wstring, wstring>& property,
      vector<wstring>& errorMessages)
{
   if (property.first == L"Email")
       dataObject.SetAddress(property.second);
   else if (property.first == L"Password")
       dataObject.SetPassword(property.second);
   else if (property.first == L"SmtpAddress")
       dataObject.SetSmtpServer(property.second);
   else if (property.first == L"SmtpPort")
       dataObject.SetSmtpPort(StringTools::ToInt(property.second));
   else if (property.first == L"UseSSL")
       dataObject.SetUseSsl(property.second == L"true");
   else
       errorMessages.push_back(ConfigurationTools::CreateUnhandledProperty(property.first));

}

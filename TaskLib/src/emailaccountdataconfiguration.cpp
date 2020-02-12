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
    map<wstring, wstring>::iterator itProp = confObject->propertyList.begin();
    map<wstring, wstring>::iterator endProp = confObject->propertyList.end();
    for (; itProp != endProp; itProp++)
    {
        pair<wstring, wstring> currentProp = *itProp;
        LoadProperty(currentProp, errorMessages);
    }
}

void EmailAccountDataConfiguration::SaveToFile(wofstream& fileStream)
{
    if (dataObject.IsValid())
    {
       ConfigurationTools::SaveValueToFile(fileStream, L"Email", dataObject.GetAddress());
       ConfigurationTools::SaveValueToFile(fileStream, L"Password", dataObject.GetPassword());
       ConfigurationTools::SaveValueToFile(fileStream, L"SmtpAddress", dataObject.GetSmtpServer());
       ConfigurationTools::SaveValueToFile(fileStream, L"SmtpPort", dataObject.GetSmtpPort());
       ConfigurationTools::SaveValueToFile(fileStream, L"UseSSL", dataObject.GetUseSsl());
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

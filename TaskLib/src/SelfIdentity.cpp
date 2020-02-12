#include "SelfIdentity.h"

#include <stdlib.h>
#include "configurationtools.h"
#include "emailaccountdataconfiguration.h"
#include "stringtools.h"

using namespace std;

Agent::Agent() : name(L""),
   reportFile(L""), reportFolder(L""),
   isDispatcherVerbose(false),
   shouldOutputDispatcherDebugInformation(false)
{
}

Agent::Agent(const Agent& other)
   : name(other.name),
     reportFile(other.reportFile), reportFolder(other.reportFolder),
     emailData(other.emailData),
     isDispatcherVerbose(other.isDispatcherVerbose),
     shouldOutputDispatcherDebugInformation(other.shouldOutputDispatcherDebugInformation)
{
}

Agent::Agent(ConfigurationObject *confObject, vector<wstring> &errorMessages)
{
   LoadSubobjects(confObject, errorMessages);
   LoadProperties(confObject, errorMessages);
}

void Agent::SaveToOpenedFile(wofstream& fileStream)
{
   fileStream << "Agent" << endl;
   fileStream << "{" << endl;
   ConfigurationTools::SaveValueToFile(fileStream, L"Name", name);
   ConfigurationTools::SaveValueToFile(fileStream, L"ReportFile", reportFile);
   ConfigurationTools::SaveValueToFile(fileStream, L"ReportFolder", reportFolder);
   SaveToFile(emailData, fileStream);
   fileStream << "}" << endl;
}

wstring Agent::GetName() const
{
   return name;
}

void Agent::SetName(const wstring& value)
{
   name = value;
}

wstring Agent::GetReportFile() const
{
   return reportFile;
}

void Agent::SetReportFile(const wstring& value)
{
   reportFile = value;
}

wstring Agent::GetReportFolder() const
{
   return reportFolder;
}

void Agent::SetReportFolder(const wstring& value)
{
   reportFolder = value;
}

EmailAccountData Agent::GetEmailData() const
{
   return emailData;
}

void Agent::SetEmailData(const EmailAccountData& value)
{
   emailData = value;
}

bool Agent::HasValidEmailData() const
{
   return emailData.IsValid();
}

bool Agent::ShouldOutputDebugInformation() const
{
   return shouldOutputDispatcherDebugInformation;
}

bool Agent::IsDispatcherVerbose() const
{
   return isDispatcherVerbose;
}

void Agent::LoadSubobjects(
      ConfigurationObject* confObject,
      std::vector<wstring>& errorMessages)
{
   EmailAccountDataConfiguration emailConf(emailData);
   ConfigurationObject* emailAccountObject = confObject->GetObject(emailConf.GetName());
   if (emailAccountObject)
      emailConf.Load(emailAccountObject, errorMessages);
}

void Agent::LoadProperties(
      ConfigurationObject* confObject,
      std::vector<wstring>& errorMessages)
{
   map<wstring, wstring>::iterator itProp = confObject->propertyList.begin();
   map<wstring, wstring>::iterator endProp = confObject->propertyList.end();
   for (; itProp != endProp; itProp++)
   {
       pair<wstring, wstring> currentProp = *itProp;
       LoadProperty(currentProp, errorMessages);
   }
}

void Agent::LoadProperty(
      const pair<wstring, wstring>& property,
      vector<wstring>& errorMessages)
{
   if (property.first == L"Name")
     name = property.second;
   else if (property.first == L"ReportFile")
     reportFile = property.second;
   else if (property.first == L"ReportFolder")
     reportFolder = property.second;
   else if (property.first == L"DispatcherVerbose")
     isDispatcherVerbose = (property.second == L"yes");
   else if (property.first == L"OutputDebugInformation")
     shouldOutputDispatcherDebugInformation = (property.second == L"yes");
   else
     errorMessages.push_back(ConfigurationTools::CreateUnhandledProperty(property.first));
}

void Agent::SaveToFile(
   const EmailAccountData& emailData,
   std::wofstream& fileStream
)
{
   if (emailData.IsValid())
   {
      ConfigurationTools::SaveValueToFile(fileStream, L"Email", emailData.GetAddress());
      ConfigurationTools::SaveValueToFile(fileStream, L"Password", emailData.GetPassword());
      ConfigurationTools::SaveValueToFile(fileStream, L"SmtpAddress", emailData.GetSmtpServer());
      ConfigurationTools::SaveValueToFile(fileStream, L"SmtpPort", emailData.GetSmtpPort());
      ConfigurationTools::SaveValueToFile(fileStream, L"UseSSL", emailData.GetUseSsl());
   }
}

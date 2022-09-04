#include "SelfIdentity.h"

#include <stdlib.h>
#include "configurationtools.h"
#include "emailaccountdataconfiguration.h"
#include "stringtools.h"
#include "telegrambotconfiguration.h"

using namespace std;

Agent::Agent() :
   name(L""),
   reportFile(L""), reportFolder(L""),
   isDispatcherVerbose(false),
   outputDispatcherDebugInformation(DebugOutput::NEVER),
   botData(NULL)
{
}

Agent::Agent(const Agent& other)
   : name(other.name),
     reportFile(other.reportFile), reportFolder(other.reportFolder),
     emailData(other.emailData),
     isDispatcherVerbose(other.isDispatcherVerbose),
     outputDispatcherDebugInformation(other.outputDispatcherDebugInformation),
     botData(other.botData)
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

   EmailAccountDataConfiguration::SaveToFile(emailData, fileStream);

   ConfigurationTools::SaveValueToFile(fileStream, L"DispatcherVerbose", isDispatcherVerbose ? L"true" : L"false");
   ConfigurationTools::SaveValueToFile(fileStream, L"OutputDebugInformation",
                                       DebugOutput::GetDescription(outputDispatcherDebugInformation));
   if (botData)
      TelegramBotConfiguration::SaveToFile(*botData, fileStream);
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

int Agent::GetOutputDebugInformation() const
{
   return outputDispatcherDebugInformation;
}

bool Agent::IsDispatcherVerbose() const
{
   return isDispatcherVerbose;
}

bool Agent::HasBot() const
{
   return botData != NULL;
}

TelegramBotData* Agent::GetBotData()
{
   return botData;
}

void Agent::LoadSubobjects(
      ConfigurationObject* confObject,
      std::vector<wstring>& errorMessages)
{
   EmailAccountDataConfiguration emailConf(emailData);
   ConfigurationObject* emailAccountObject = confObject->GetObject(emailConf.GetName());
   if (emailAccountObject)
      emailConf.Load(emailAccountObject, errorMessages);

   ConfigurationObject* telegramConfObject = confObject->GetObject(TelegramBotConfiguration::GetName());
   if (telegramConfObject)
      botData = TelegramBotConfiguration::Load(telegramConfObject, errorMessages);
}

void Agent::LoadProperties(
      ConfigurationObject* confObject,
      std::vector<wstring>& errorMessages)
{
   map<wstring, wstring>::iterator itProp = confObject->BeginProperties();
   map<wstring, wstring>::iterator endProp = confObject->EndProperties();
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
     isDispatcherVerbose = (property.second == L"true");
   else if (property.first == L"OutputDebugInformation")
     outputDispatcherDebugInformation = DebugOutput::GetValue(property.second);
   else
     errorMessages.push_back(ConfigurationTools::CreateUnhandledProperty(property.first));
}

int Agent::ParseBotMode(const wstring& desc)
{
   if (desc == L"telegram")
      return BOTMODE_TELEGRAM;
   else
      return BOTMODE_NO;
}

wstring Agent::GetBotModeName(const int value)
{
   if (value == BOTMODE_TELEGRAM)
      return L"telegram";
   else
      return L"no";
}

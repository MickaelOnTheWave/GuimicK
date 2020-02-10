#include "SelfIdentity.h"

#include <stdlib.h>
#include "configurationtools.h"
#include "stringtools.h"

using namespace std;

Agent::Agent() : name(L""),
   reportFile(L""), reportFolder(L"")
{
}

Agent::Agent(const Agent& other)
   : name(other.name),
     reportFile(other.reportFile), reportFolder(other.reportFolder),
     emailData(other.emailData)
{
}

Agent::Agent(ConfigurationObject *confObject, vector<wstring> &errorMessages)
{
    map<wstring, wstring>::iterator itProp = confObject->propertyList.begin();
    map<wstring, wstring>::iterator endProp = confObject->propertyList.end();
    for (; itProp != endProp; itProp++)
    {
        pair<wstring, wstring> currentProp = *itProp;

        if (currentProp.first == L"Name")
            name = currentProp.second;
        else if (currentProp.first == L"Email")
            emailData.SetAddress(currentProp.second);
        else if (currentProp.first == L"Password")
            emailData.SetPassword(currentProp.second);
        else if (currentProp.first == L"SmtpAddress")
            emailData.SetSmtpServer(currentProp.second);
        else if (currentProp.first == L"SmtpPort")
            emailData.SetSmtpPort(StringTools::ToInt(currentProp.second));
        else if (currentProp.first == L"UseSSL")
            emailData.SetUseSsl(currentProp.second == L"true");
        else if (currentProp.first == L"ReportFile")
            reportFile = currentProp.second;
        else if (currentProp.first == L"ReportFolder")
            reportFolder = currentProp.second;
        else
            errorMessages.push_back(BuildUnhandledPropertyMessage(currentProp.first));
    }
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

wstring Agent::BuildUnhandledPropertyMessage(const wstring &property) const
{
    wstring message = L"Warning : unhandled property ";
    message += property + L" for Agent configuration";
    return message;
}

#include "SelfIdentity.h"

#include <stdlib.h>
#include "configurationtools.h"

using namespace std;

Agent::Agent() : name(""),
   reportFile(""), reportFolder("")
{
}

Agent::Agent(const Agent& other)
   : name(other.name),
     reportFile(other.reportFile), reportFolder(other.reportFolder),
     emailData(other.emailData)
{
}

Agent::Agent(ConfigurationObject *confObject, vector<string> &errorMessages)
{
    map<string, string>::iterator itProp = confObject->propertyList.begin();
    map<string, string>::iterator endProp = confObject->propertyList.end();
    for (; itProp != endProp; itProp++)
    {
        pair<string, string> currentProp = *itProp;

        if (currentProp.first == "Name")
            name = currentProp.second;
        else if (currentProp.first == "Email")
            emailData.SetAddress(currentProp.second);
        else if (currentProp.first == "Password")
            emailData.SetPassword(currentProp.second);
        else if (currentProp.first == "SmtpAddress")
            emailData.SetSmtpServer(currentProp.second);
        else if (currentProp.first == "SmtpPort")
            emailData.SetSmtpPort(atoi(currentProp.second.c_str()));
        else if (currentProp.first == "UseSSL")
            emailData.SetUseSsl(currentProp.second == "true");
        else if (currentProp.first == "ReportFile")
            reportFile = currentProp.second;
        else if (currentProp.first == "ReportFolder")
            reportFolder = currentProp.second;
        else
            errorMessages.push_back(BuildUnhandledPropertyMessage(currentProp.first));
    }
}

void Agent::SaveToOpenedFile(ofstream& fileStream)
{
   fileStream << "Agent" << endl;
   fileStream << "{" << endl;
   ConfigurationTools::SaveValueToFile(fileStream, "Name", name);
   emailData.SaveToOpenedFile(fileStream);

   fileStream << "}" << endl;
}

string Agent::GetName() const
{
   return name;
}

string Agent::GetReportFile() const
{
   return reportFile;
}

string Agent::GetReportFolder() const
{
   return reportFolder;
}

EmailData Agent::GetEmailData() const
{
   return emailData;
}

bool Agent::HasValidEmailData() const
{
   return emailData.IsValid();
}

string Agent::BuildUnhandledPropertyMessage(const string &property) const
{
    string message = "Warning : unhandled property ";
    message += property + " for Agent configuration";
    return message;
}

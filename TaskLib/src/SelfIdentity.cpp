#include "SelfIdentity.h"

#include <stdlib.h>

using namespace std;

SelfIdentity::SelfIdentity()
{
    name = "";
    email = "";
    emailPassword = "";
    emailSmtpServer = "";
    emailUseSsl = false;
    emailSmtpPort = -1;
}

SelfIdentity::SelfIdentity(const SelfIdentity& other)
   : name(other.name),
     email(other.email),
     emailPassword(other.emailPassword),
     emailSmtpServer(other.emailSmtpServer),
     emailUseSsl(other.emailUseSsl),
     emailSmtpPort(other.emailSmtpPort)
{
}

SelfIdentity::SelfIdentity(ConfigurationObject *confObject,
                           vector<string> &errorMessages)
{
    map<string, string>::iterator itProp = confObject->propertyList.begin();
    map<string, string>::iterator endProp = confObject->propertyList.end();
    for (; itProp != endProp; itProp++)
    {
        pair<string, string> currentProp = *itProp;

        if (currentProp.first == "Name")
            name = currentProp.second;
        else if (currentProp.first == "Email")
            email = currentProp.second;
        else if (currentProp.first == "Password")
            emailPassword = currentProp.second;
        else if (currentProp.first == "SmtpAddress")
            emailSmtpServer = currentProp.second;
        else if (currentProp.first == "SmtpPort")
            emailSmtpPort = atoi(currentProp.second.c_str());
        else if (currentProp.first == "UseSSL")
            emailUseSsl = (currentProp.second == "true");
        else
            errorMessages.push_back(BuildUnhandledPropertyMessage(currentProp.first));
    }
}

bool SelfIdentity::HasValidEmailData() const
{
    if (email == "")
        return false;
    else if (emailPassword == "")
        return false;
    else if (emailSmtpPort == -1)
        return false;
    else if (emailSmtpServer == "")
        return false;
    else
        return true;
}

string SelfIdentity::BuildUnhandledPropertyMessage(const string &property) const
{
    string message = "Warning : unhandled property ";
    message += property + " for Agent configuration";
    return message;
}

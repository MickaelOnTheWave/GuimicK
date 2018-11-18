#include "emaildata.h"

#include "configurationtools.h"

EmailData::EmailData() :
   address(""), password(""), smtpServer(""),
   useSsl(false), smtpPort(-1)
{
}

EmailData::EmailData(const EmailData& other) :
   address(other.address),
   password(other.password),
   smtpServer(other.smtpServer),
   useSsl(other.useSsl),
   smtpPort(other.smtpPort)
{
}

void EmailData::SaveToOpenedFile(std::ofstream& fileStream)
{
   if (IsValid())
   {
      ConfigurationTools::SaveValueToFile(fileStream, "Email", address);
      ConfigurationTools::SaveValueToFile(fileStream, "Password", password);
      ConfigurationTools::SaveValueToFile(fileStream, "SmtpAddress", smtpServer);
      ConfigurationTools::SaveValueToFile(fileStream, "SmtpPort", smtpPort);
      ConfigurationTools::SaveValueToFile(fileStream, "UseSSL", useSsl);
   }
}

std::string EmailData::GetAddress() const
{
   return address;
}

void EmailData::SetAddress(const std::string& value)
{
   address = value;
}

std::string EmailData::GetPassword() const
{
   return password;
}

void EmailData::SetPassword(const std::string& value)
{
   password = value;
}

std::string EmailData::GetSmtpServer() const
{
   return smtpServer;
}

void EmailData::SetSmtpServer(const std::string& value)
{
   smtpServer = value;
}

int EmailData::GetSmtpPort() const
{
   return smtpPort;
}

void EmailData::SetSmtpPort(int value)
{
   smtpPort = value;
}

bool EmailData::GetUseSsl() const
{
   return useSsl;
}

void EmailData::SetUseSsl(bool value)
{
   useSsl = value;
}

bool EmailData::IsValid() const
{
   if (address == "")
       return false;
   else if (password == "")
       return false;
   else if (smtpPort == -1)
       return false;
   else if (smtpServer == "")
       return false;
   else
       return true;
}

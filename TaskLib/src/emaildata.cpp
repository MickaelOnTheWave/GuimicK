#include "emaildata.h"

#include "configurationtools.h"

EmailData::EmailData() :
   address(L""), password(L""), smtpServer(L""),
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

void EmailData::SaveToOpenedFile(std::wofstream& fileStream)
{
   if (IsValid())
   {
      ConfigurationTools::SaveValueToFile(fileStream, L"Email", address);
      ConfigurationTools::SaveValueToFile(fileStream, L"Password", password);
      ConfigurationTools::SaveValueToFile(fileStream, L"SmtpAddress", smtpServer);
      ConfigurationTools::SaveValueToFile(fileStream, L"SmtpPort", smtpPort);
      ConfigurationTools::SaveValueToFile(fileStream, L"UseSSL", useSsl);
   }
}

std::wstring EmailData::GetAddress() const
{
   return address;
}

void EmailData::SetAddress(const std::wstring& value)
{
   address = value;
}

std::wstring EmailData::GetPassword() const
{
   return password;
}

void EmailData::SetPassword(const std::wstring& value)
{
   password = value;
}

std::wstring EmailData::GetSmtpServer() const
{
   return smtpServer;
}

void EmailData::SetSmtpServer(const std::wstring& value)
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
   if (address == L"")
       return false;
   else if (password == L"")
       return false;
   else if (smtpPort == -1)
       return false;
   else if (smtpServer == L"")
       return false;
   else
       return true;
}

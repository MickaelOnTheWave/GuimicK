#ifndef EMAILDATA_H
#define EMAILDATA_H

#include <fstream>
#include <string>

class EmailData
{
public:
   EmailData();
	EmailData(const EmailData& other);

   void SaveToOpenedFile(std::ofstream& fileStream);

   std::string GetAddress() const;
   void SetAddress(const std::string& value);

   std::string GetPassword() const;
   void SetPassword(const std::string& value);

   std::string GetSmtpServer() const;
   void SetSmtpServer(const std::string& value);

   int GetSmtpPort() const;
   void SetSmtpPort(int value);

   bool GetUseSsl() const;
   void SetUseSsl(bool value);

	bool IsValid() const;

private:
   std::string address;
   std::string password;
   std::string smtpServer;
   bool useSsl;
   int smtpPort;
};

#endif // EMAILDATA_H

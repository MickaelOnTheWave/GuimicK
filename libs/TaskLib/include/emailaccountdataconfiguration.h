#ifndef EMAILACCOUNTDATACONFIGURATION_H
#define EMAILACCOUNTDATACONFIGURATION_H

#include "configurationobject.h"
#include "emailaccountdata.h"

class EmailAccountDataConfiguration
{
public:
   EmailAccountDataConfiguration(EmailAccountData& _dataObject);

   std::wstring GetName() const;

   void Load(ConfigurationObject *confObject,
             std::vector<std::wstring> &errorMessages);

   static void SaveToFile(const EmailAccountData& data, std::wofstream& fileStream);

private:
   void LoadProperty(const std::pair<std::wstring, std::wstring>& property,
                     std::vector<std::wstring> &errorMessages);

   EmailAccountData& dataObject;
};

#endif // EMAILACCOUNTDATACONFIGURATION_H

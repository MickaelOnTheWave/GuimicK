#ifndef TELEGRAMBOTCONFIGURATION_H
#define TELEGRAMBOTCONFIGURATION_H

#include <vector>

#include "configurationobject.h"
#include "TelegramBotData.h"

class TelegramBotConfiguration
{
public:
   static std::wstring GetName();

   static TelegramBotData* Load(ConfigurationObject* confObject,
             std::vector<std::wstring> &errorMessages);

   static void SaveToFile(const TelegramBotData& data, std::wofstream& fileStream);

protected:
   static void CheckUnhandledProperties(ConfigurationObject* confObject,
                                        const std::vector<std::wstring>& handledProperties,
                                        std::vector<std::wstring>& errorMessages);
};

#endif // TELEGRAMBOTCONFIGURATION_H

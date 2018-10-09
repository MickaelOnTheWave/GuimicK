#ifndef CONFIGURATIONTOOLS_H
#define CONFIGURATIONTOOLS_H

#include <list>
#include <string>

#include "abstractjob.h"

class ConfigurationTools
{
public:
   static std::string CreateWarning(const std::string& message);
   static std::string CreateError(const std::string& message);

   template <class T>
   static void SaveValueToFile(std::ofstream& file,
                               const std::string& name,
                               const T value);

   static void SaveJobListToFile(std::ofstream& file,
                                 const std::list<AbstractJob*>& jobList);

   static bool GetBooleanValue(const std::string& strValue,
                               std::vector<std::string>& errorMessages);

private:
   static std::string CreateMessage(const std::string& tag,
                                    const std::string& message);
};

#endif // CONFIGURATIONTOOLS_H

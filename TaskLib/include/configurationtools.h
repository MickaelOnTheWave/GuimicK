#ifndef CONFIGURATIONTOOLS_H
#define CONFIGURATIONTOOLS_H

#include <fstream>
#include <list>
#include <string>

#include "abstractjob.h"

class ConfigurationTools
{
public:
   static std::wstring CreateWarning(const std::wstring& message);
   static std::wstring CreateError(const std::wstring& message);

   template <class T>
   static void SaveValueToFile(std::wofstream& file,
                               const std::wstring& name,
                               const T value)
   {
      const std::wstring tab = L"\t";
      file << tab << name << L" = \"" << value << L"\";" << std::endl;
   }

   static void SaveJobListToFile(std::ofstream& file,
                                 const std::list<AbstractJob*>& jobList);

   static bool GetBooleanValue(const std::wstring& strValue,
                               std::vector<std::wstring>& errorMessages);

private:
   static std::wstring CreateMessage(const std::wstring& tag,
                                    const std::wstring& message);
};

#endif // CONFIGURATIONTOOLS_H

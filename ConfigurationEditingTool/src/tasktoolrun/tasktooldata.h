#ifndef TASKTOOLDATA_H
#define TASKTOOLDATA_H

#include <QString>

class TaskToolConfigurationData
{
public:
   TaskToolConfigurationData() = default;
   TaskToolConfigurationData(const QString& _configurationFile,
                             const QString& _runPath,
                             const QString& _executable)
      : configurationFile(_configurationFile),
        runPath(_runPath),
        executable(_executable)
   {
   }

   QString configurationFile = "";
   QString runPath = "";
   QString executable = "";
};

struct TaskToolRunData
{
   bool isOk = false;
   int returnCode = -1;
   std::wstring output = L"";
};

#endif // TASKTOOLDATA_H

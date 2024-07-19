#ifndef WorkExecutionManager_H
#define WorkExecutionManager_H

#include <string>
#include "clientworkmanager.h"
#include "standaloneconfiguration.h"

class WorkExecutionManager
{
public:
   WorkExecutionManager(StandaloneConfiguration& _configuration,
                        const std::vector<std::wstring>& _configurationErrors,
                        ClientWorkManager* _worklist, const std::wstring& _appVersion);

   AbstractReportCreator* Run();
   void Run(int& currentJobIndex, AbstractReportCreator* reportCreator);

   StandaloneConfiguration& configuration;
   const std::vector<std::wstring>& configurationErrors;
   ClientWorkManager* worklist;
   const std::wstring appVersion;
};

#endif // WorkExecutionManager_H

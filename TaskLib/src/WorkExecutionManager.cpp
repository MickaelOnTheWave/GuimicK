#include "WorkExecutionManager.h"

WorkExecutionManager::WorkExecutionManager(StandaloneConfiguration& _configuration,
                                           const std::vector<std::wstring>& _configurationErrors,
                                           ClientWorkManager* _worklist, const std::wstring& _appVersion)
   : configuration(_configuration), configurationErrors(_configurationErrors),
     worklist(_worklist), appVersion(_appVersion)
{
}

AbstractReportCreator* WorkExecutionManager::Run()
{
   WorkResultData* workResult = worklist->RunWorkList();
   AbstractReportCreator* reportCreator = configuration.GetReportCreator();
   reportCreator->Generate(workResult, configurationErrors, appVersion);
   delete workResult;

   return reportCreator;
}

AbstractReportCreator* WorkExecutionManager::Run(int& currentJobIndex)
{
   WorkResultData* workResult = worklist->RunWorkList(currentJobIndex);
   AbstractReportCreator* reportCreator = configuration.GetReportCreator();
   reportCreator->Generate(workResult, configurationErrors, appVersion);
   delete workResult;

   return reportCreator;
}

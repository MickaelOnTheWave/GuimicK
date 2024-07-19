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
   int dummyIndex;
   AbstractReportCreator* reportCreator = configuration.GetReportCreator();
   Run(dummyIndex, reportCreator);
   return reportCreator;
}

void WorkExecutionManager::Run(int& currentJobIndex, AbstractReportCreator* reportCreator)
{
   WorkResultData* workResult = worklist->RunWorkList(currentJobIndex);
   if (reportCreator == nullptr)
      reportCreator = configuration.GetReportCreator();
   reportCreator->Generate(workResult, configurationErrors, appVersion);
   delete workResult;
}

#ifndef MAINTOOLMODULE_H
#define MAINTOOLMODULE_H

#include "abstractreportcreator.h"
#include "abstractreportdispatcherreplacer.h"
#include "clientworkmanager.h"
#include "commandlinemanager.h"
#include "configuration.h"

class MainToolModule
{
public:
    MainToolModule(const std::string& _version);
    ~MainToolModule();

    void EnableTimedRuns(const bool value);
    void SetDispatcherReplacer(AbstractReportDispatcherReplacer* _replacer);

    int RunFromCommandLine(int argc, char* argv[]);
    int RunFromParameterMap(const std::map<std::string, std::string>& parameters);

private:

    int Run(CommandLineManager& commandLine);

    bool SetupCommandLine(CommandLineManager& manager);

    std::string GetConfigurationFile(const std::string& commandLineFile);

    void ShowErrors(std::vector<std::string> &errorMessages);

    bool SetupShutdownOptions(const bool isConfigShutdownEnabled, const bool isCommandLineShutdownCanceled,
                              ClientWorkManager* workList);

    void SetupSingleJobOption(ClientWorkManager* workList, const CommandLineManager& commandLine);

    AbstractReportCreator* RunWorkList(ClientWorkManager* workList, const Configuration& configuration,
                                       const std::vector<std::string>& configurationErrors);

    void DispatchReport(AbstractReportCreator* reportCreator,
                        const Configuration& configuration,
                        const CommandLineManager& commandLine);

    void DispatchUsingFallback(AbstractReportCreator *reportCreator,
                               const Configuration& configuration);

    AbstractReportDispatcher* ReplaceDispatcherIfEmail(AbstractReportDispatcher* input,
                                                       const Configuration& configuration);

    bool RunLocalShutdown(const bool isLocalShutdownEnabled);

    const std::string version;
    AbstractReportDispatcherReplacer* replacer;
    bool timedRuns;
};

#endif // MAINTOOLMODULE_H

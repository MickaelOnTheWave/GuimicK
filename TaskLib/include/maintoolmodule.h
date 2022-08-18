#ifndef MAINTOOLMODULE_H
#define MAINTOOLMODULE_H

#include "abstractreportcreator.h"
#include "abstractreportdispatcherreplacer.h"
#include "clientworkmanager.h"
#include "commandlinemanager.h"
#include "consolejob.h"
#include "standaloneconfiguration.h"

class MainToolModule
{
public:
    MainToolModule(const std::wstring& _version);
    ~MainToolModule();

    void EnableTimedRuns(const bool value);
    void SetDispatcherReplacer(AbstractReportDispatcherReplacer* _replacer);
    void SetFallbackDispatcher(AbstractReportDispatcher* dispatcher);

    int RunFromCommandLine(int argc, char* argv[]);
    int RunFromCommandLine(int argc, wchar_t* argv[]);
    int RunFromParameterMap(const std::map<std::wstring, std::wstring>& parameters);

private:

    int Run(CommandLineManager& commandLine);

    bool SetupCommandLine(CommandLineManager& manager);

    std::wstring GetConfigurationFile(const std::wstring& commandLineFile);

    void ShowErrors(std::vector<std::wstring> &errorMessages);

    bool SetupShutdownOptions(const bool isConfigShutdownEnabled, const bool isCommandLineShutdownCanceled,
                              ClientWorkManager* workList);

    void SetupSingleJobOption(ClientWorkManager* workList, const CommandLineManager& commandLine);

    int RunBotMode(ClientWorkManager* workList);

    AbstractReportCreator* RunWorkList(ClientWorkManager* workList, const StandaloneConfiguration& configuration,
                                       const std::vector<std::wstring>& configurationErrors);

    bool DispatchReport(AbstractReportCreator* reportCreator,
                        const StandaloneConfiguration& configuration,
                        const CommandLineManager& commandLine);

    bool DispatchReport(AbstractReportCreator* reportCreator,
                        AbstractReportDispatcher* dispatcher);

    AbstractReportDispatcher* ReplaceDispatcherIfEmail(AbstractReportDispatcher* input,
                                                       const StandaloneConfiguration& configuration);

    bool RunLocalShutdown(const bool isLocalShutdownEnabled);

    ConsoleJob* CreateLocalShutdownJob() const;

    const std::wstring version;
    AbstractReportDispatcherReplacer* replacer;
    AbstractReportDispatcher* fallbackDispatcher;
    bool timedRuns;
    bool isWindowsComInitialized;
};

#endif // MAINTOOLMODULE_H

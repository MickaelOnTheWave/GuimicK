#include "maintoolmodule.h"

#ifdef _WIN32
   #include <comdef.h>
#endif

#include <iostream>
#include "consolejob.h"
#include "filereportdispatcher.h"
#include "filetools.h"
#include "standaloneconfiguration.h"
#include "taskmanagerconfiguration.h"

using namespace std;

static const wstring DEFAULT_CONFIGURATION_FILE = L"configuration.txt";

static const wstring noEmailCommand = L"noemail";
static const wstring noShutdownCommand = L"noshutdown";
static const wstring onlyOneJobCommand = L"onlyjob";
static const wstring confFileCommand = L"conffile";

static const int TM_NO_ERROR           = 0;
static const int CONFIGURATION_ERROR   = 1;
static const int SHUTDOWN_ERROR        = 2;
static const int WINDOWS_INIT_ERROR    = 3;

MainToolModule::MainToolModule(const wstring &_version)
    : version(_version), replacer(NULL), timedRuns(true)
{
    fallbackDispatcher = new FileReportDispatcher();

#ifdef _WIN32
    const HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    isWindowsComInitialized = (SUCCEEDED(hr));
#endif
}

MainToolModule::~MainToolModule()
{
    delete replacer;
    delete fallbackDispatcher;
#ifdef _WIN32
    if (isWindowsComInitialized)
       CoUninitialize();
#endif
}

void MainToolModule::EnableTimedRuns(const bool value)
{
    timedRuns = value;
}

void MainToolModule::SetDispatcherReplacer(AbstractReportDispatcherReplacer *_replacer)
{
    replacer = _replacer;
}

void MainToolModule::SetFallbackDispatcher(AbstractReportDispatcher *dispatcher)
{
    delete fallbackDispatcher;
    fallbackDispatcher = dispatcher;
}

int MainToolModule::RunFromCommandLine(int argc, char *argv[])
{
    CommandLineManager commandLine(argc, argv);
    return Run(commandLine);
}

int MainToolModule::RunFromCommandLine(int argc, wchar_t* argv[])
{
   CommandLineManager commandLine(argc, argv);
   return Run(commandLine);
}

int MainToolModule::RunFromParameterMap(const std::map<wstring, wstring> &parameters)
{
    CommandLineManager commandLine(parameters);
    return Run(commandLine);
}

int MainToolModule::Run(CommandLineManager &commandLine)
{
#ifdef _WIN32
   if (isWindowsComInitialized == false)
      return WINDOWS_INIT_ERROR;
#endif

    const bool shouldReturn = SetupCommandLine(commandLine);
    if (shouldReturn)
        return NO_ERROR;

    wstring configurationFile = GetConfigurationFile(commandLine.GetParameterValue(confFileCommand));
    if (FileTools::FileExists(configurationFile) == false)
    {
        wcout << "Error : file " << configurationFile << " could not be opened." << endl;
        return CONFIGURATION_ERROR;
    }

    TaskManagerConfiguration configuration;
    vector<wstring> configurationErrors;
    bool configurationIsUsable = configuration.LoadFromFile(configurationFile, configurationErrors);
    if (configurationIsUsable == false)
    {
        ShowErrors(configurationErrors);
        cout << "Unrecoverable errors while trying to read configuration file. Exiting." << endl;
        return CONFIGURATION_ERROR;
    }

    StandaloneConfiguration* typedConfiguration = dynamic_cast<StandaloneConfiguration*>(configuration.GetTypeConfiguration());
    ClientWorkManager* workList = typedConfiguration->BuildWorkList(timedRuns);
    bool localShutdown = SetupShutdownOptions(typedConfiguration->GetLocalShutdown(),
                                              commandLine.HasParameter(noShutdownCommand),
                                              workList);

    SetupSingleJobOption(workList, commandLine);

    AbstractReportCreator* reportCreator = RunWorkList(workList, *typedConfiguration, configurationErrors);
    DispatchReport(reportCreator, *typedConfiguration, commandLine);
    delete reportCreator;

    const bool ok = RunLocalShutdown(localShutdown);
    return (ok) ? TM_NO_ERROR : SHUTDOWN_ERROR;
}

bool MainToolModule::SetupCommandLine(CommandLineManager& manager)
{
    manager.AddParameter(noEmailCommand,     L"Doesn't send report via email.");
    manager.AddParameter(noShutdownCommand,  L"Doesn't shutdown client neither server after running.");
    manager.AddParameter(onlyOneJobCommand,  L"[JOBNAME] Only runs job JOBNAME.");
    manager.AddParameter(confFileCommand,    L"[CONFIGURATION FILE] Specifies which configuration file to use.");

    manager.EnableHelpCommand();
    manager.EnableVersionCommand(L"Task Manager", version, L"Mickaël C. Guimarães", L"2014-2019");

    return (manager.HandleUnknownParameters() || manager.HandleVersionCommand() ||
            manager.HandleHelpCommand());
}

wstring MainToolModule::GetConfigurationFile(const wstring& commandLineFile)
{
    return (commandLineFile != L"") ? commandLineFile : DEFAULT_CONFIGURATION_FILE;
}

void MainToolModule::ShowErrors(vector<wstring>& errorMessages)
{
    if (errorMessages.size() == 0)
        return;

    vector<wstring>::const_iterator it=errorMessages.begin();
    for (; it!=errorMessages.end(); it++)
        wcout << *it << endl;
    wcout << endl;
}

bool MainToolModule::SetupShutdownOptions(const bool isConfigShutdownEnabled,
                                          const bool isCommandLineShutdownCanceled,
                                          ClientWorkManager* workList)
{
    bool localShutdown = isConfigShutdownEnabled;
    if (isCommandLineShutdownCanceled)
    {
        localShutdown = false;
        workList->RemoveJob(L"Shutdown");
    }
    return localShutdown;
}

void MainToolModule::SetupSingleJobOption(ClientWorkManager* workList,
                                          const CommandLineManager& commandLine)
{
    wstring singleJob = commandLine.GetParameterValue(onlyOneJobCommand);
    if (singleJob != L"")
        workList->RemoveAllButJobs(singleJob);
}

AbstractReportCreator* MainToolModule::RunWorkList(ClientWorkManager* workList,
                                                   const StandaloneConfiguration& configuration,
                                                   const vector<wstring>& configurationErrors)
{
    WorkResultData* workResult = workList->RunWorkList();
    AbstractReportCreator* reportCreator = configuration.GetReportCreator();
    reportCreator->Generate(workResult, configurationErrors, version);
    delete workResult;

    return reportCreator;
}

void MainToolModule::DispatchReport(AbstractReportCreator* reportCreator,
                    const StandaloneConfiguration& configuration,
                    const CommandLineManager& commandLine)
{
    AbstractReportDispatcher* reportDispatcher = configuration.CreateReportDispatcher(
                commandLine.HasParameter(noEmailCommand)
                );

    if (replacer)
        reportDispatcher = replacer->Run(reportDispatcher, configuration);

    bool ok = DispatchReport(reportCreator, reportDispatcher);
    if (!ok)
        DispatchReport(reportCreator, fallbackDispatcher);
}

bool MainToolModule::DispatchReport(AbstractReportCreator *reportCreator,
                                    AbstractReportDispatcher *dispatcher)
{
    bool ok = dispatcher->Dispatch(reportCreator);
    if (!ok)
    {
        const wstring fallbackName = (dispatcher != fallbackDispatcher) ?
                    fallbackDispatcher->GetName() :
                    wstring(L"");

        reportCreator->UpdateWithDispatchError(dispatcher->GetName(), fallbackName);
    }
    return ok;
}

bool MainToolModule::RunLocalShutdown(const bool isLocalShutdownEnabled)
{
    if (isLocalShutdownEnabled == false)
        return true;

    ConsoleJob finalShutdown(L"/sbin/poweroff");
    JobStatus* status = finalShutdown.Run();
    const bool shutdownError = (status->GetCode() != JobStatus::Ok);
    if (shutdownError)
        wcout << "Local shutdown failed : " << status->GetDescription() << endl;

    delete status;
    return !shutdownError;
}

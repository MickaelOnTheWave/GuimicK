#include "maintoolmodule.h"

#ifdef _WIN32
   #include <comdef.h>
#endif

#include <iostream>

#include "botfactory.h"
#include "consolejob.h"
#include "filereportdispatcher.h"
#include "filetools.h"
#include "Bot/runningbot.h"
#include "standaloneconfiguration.h"
#include "stringtools.h"
#include "taskmanagerconfiguration.h"


using namespace std;

static const wstring DEFAULT_CONFIGURATION_FILE = L"configuration.txt";

static const string noEmailCommand = "noemail";
static const string noShutdownCommand = "noshutdown";
static const string onlyOneJobCommand = "onlyjob";
static const string confFileCommand = "conffile";

static const int TM_NO_ERROR           = 0;
static const int CONFIGURATION_ERROR   = 1;
static const int SHUTDOWN_ERROR        = 2;
static const int WINDOWS_INIT_ERROR    = 3;
static const int DISPATCH_ERROR        = 4;

MainToolModule::MainToolModule(const wstring &_version)
    : version(_version), replacer(NULL), timedRuns(true)
{
#ifdef _WIN32
   fallbackDispatcher = nullptr;
#else
    fallbackDispatcher = new FileReportDispatcher();
#endif

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

int MainToolModule::RunFromParameterMap(const map<wstring, wstring> &parameters)
{
   map<string, string> convertedParams;
   for (const auto& param : parameters)
   {
      const string key = StringTools::UnicodeToUtf8(param.first);
      const string value = StringTools::UnicodeToUtf8(param.second);
      convertedParams.insert(make_pair(key, value));
   }

   CommandLineManager commandLine(convertedParams);
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
        return TM_NO_ERROR;


    const string paramValue = commandLine.GetParameterValue(confFileCommand);
    wstring configurationFile = GetConfigurationFile(StringTools::Utf8ToUnicode(confFileCommand));
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

    WorkExecutionManager workManager(*typedConfiguration, configurationErrors, workList, version);

    if (typedConfiguration->GetAgent()->HasBot())
       return RunBotMode(workManager);
    else
    {
       AbstractReportCreator* reportCreator = workManager.Run();
       const bool dispatched = DispatchReport(reportCreator, *typedConfiguration, commandLine);
       delete reportCreator;

       const bool ok = RunLocalShutdown(localShutdown);
       if (ok)
          return (dispatched) ? TM_NO_ERROR : DISPATCH_ERROR;
       else
          return SHUTDOWN_ERROR;
    }
}

bool MainToolModule::SetupCommandLine(CommandLineManager& manager)
{
   const string convertedVersion = StringTools::UnicodeToUtf8(version);
   manager.AddParameter(noEmailCommand,     "Doesn't send report via email.");
   manager.AddParameter(noShutdownCommand,  "Doesn't shutdown client neither server after running.");
   manager.AddParameter(onlyOneJobCommand,  "[JOBNAME] Only runs job JOBNAME.");
   manager.AddParameter(confFileCommand,    "[CONFIGURATION FILE] Specifies which configuration file to use.");

   manager.EnableHelpCommand();
   manager.EnableVersionCommand("GuimicK", convertedVersion, "Mickaël C. Guimarães", "2014-2024");

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
   const string singleJob = commandLine.GetParameterValue(onlyOneJobCommand);
   if (singleJob != "")
   {
      const wstring convSingleJob = StringTools::Utf8ToUnicode(singleJob);
      workList->RemoveAllButJobs(convSingleJob);
   }
}

int MainToolModule::RunBotMode(WorkExecutionManager& data)
{
   RunningBot* bot = BotFactory::Create(data);
   bot->LoopRun();
   delete bot;
   return 0;
}

bool MainToolModule::DispatchReport(AbstractReportCreator* reportCreator,
                    const StandaloneConfiguration& configuration,
                    const CommandLineManager& commandLine)
{
    AbstractReportDispatcher* reportDispatcher = configuration.CreateReportDispatcher(
                commandLine.HasParameter(noEmailCommand)
                );

    if (replacer)
        reportDispatcher = replacer->Run(reportDispatcher, configuration);

    bool ok = DispatchReport(reportCreator, reportDispatcher);
    if (!ok && fallbackDispatcher)
        ok = DispatchReport(reportCreator, fallbackDispatcher);
    return ok;
}

bool MainToolModule::DispatchReport(AbstractReportCreator *reportCreator,
                                    AbstractReportDispatcher *dispatcher)
{
    bool ok = dispatcher->Dispatch(reportCreator);
    if (!ok && fallbackDispatcher)
    {
        const wstring fallbackName = (dispatcher != fallbackDispatcher) ?
                    fallbackDispatcher->GetName() :
                    wstring(L"");

        reportCreator->UpdateWithDispatchError(
                 dispatcher->GetName(), dispatcher->GetLastError(),
                 fallbackName);
    }
    return ok;
}

bool MainToolModule::RunLocalShutdown(const bool isLocalShutdownEnabled)
{
    if (isLocalShutdownEnabled == false)
        return true;

    ConsoleJob* finalShutdown = CreateLocalShutdownJob();
    JobStatus* status = finalShutdown->Run();
    const bool shutdownError = (status->GetCode() != JobStatus::Ok);
    if (shutdownError)
        wcout << "Local shutdown failed : " << status->GetDescription() << endl;

    delete status;
    delete finalShutdown;
    return !shutdownError;
}

ConsoleJob* MainToolModule::CreateLocalShutdownJob() const
{
#ifdef _WIN32
   return new ConsoleJob(L"shutdown.exe", L"-f -s");
#else
   return new ConsoleJob(L"/sbin/poweroff");
#endif
}

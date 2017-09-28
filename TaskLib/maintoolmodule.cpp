#include "maintoolmodule.h"

#include <iostream>
#include "consolejob.h"
#include "filereportdispatcher.h"
#include "filetools.h"

using namespace std;

static const string DEFAULT_CONFIGURATION_FILE = "configuration.txt";

static const string noEmailCommand = "noemail";
static const string noShutdownCommand = "noshutdown";
static const string onlyOneJobCommand = "onlyjob";
static const string confFileCommand = "conffile";

static const int NO_ERROR              = 0;
static const int CONFIGURATION_ERROR   = 1;
static const int OTHER_ERROR           = 2;

MainToolModule::MainToolModule(const string &_version)
    : version(_version), replacer(NULL), timedRuns(true)
{
    fallbackDispatcher = new FileReportDispatcher();
}

MainToolModule::~MainToolModule()
{
    delete replacer;
    delete fallbackDispatcher;
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

int MainToolModule::RunFromParameterMap(const std::map<string, string> &parameters)
{
    CommandLineManager commandLine(parameters);
    return Run(commandLine);
}

int MainToolModule::Run(CommandLineManager &commandLine)
{
    bool shouldReturn = SetupCommandLine(commandLine);
    if (shouldReturn)
        return NO_ERROR;

    string configurationFile = GetConfigurationFile(commandLine.GetParameterValue(confFileCommand));
    if (FileTools::FileExists(configurationFile) == false)
    {
        cout << "Error : file " << configurationFile << " could not be opened." << endl;
        return CONFIGURATION_ERROR;
    }

    ServerConfiguration configuration;
    vector<string> configurationErrors;
    bool configurationIsUsable = configuration.LoadFromFile(configurationFile, configurationErrors);
    if (configurationIsUsable == false)
    {
        ShowErrors(configurationErrors);
        cout << "Unrecoverable errors while trying to read configuration file. Exiting." << endl;
        return CONFIGURATION_ERROR;
    }

    ClientWorkManager* workList = (timedRuns) ? configuration.BuildTimedWorkList()
                                              : configuration.BuildSimpleWorkList();

    bool localShutdown = SetupShutdownOptions(configuration.GetLocalShutdown(),
                                              commandLine.HasParameter(noShutdownCommand),
                                              workList);

    SetupSingleJobOption(workList, commandLine);

    AbstractReportCreator* reportCreator = RunWorkList(workList, configuration, configurationErrors);
    DispatchReport(reportCreator, configuration, commandLine);
    delete reportCreator;

    bool ok = RunLocalShutdown(localShutdown);
    return (ok) ? NO_ERROR : OTHER_ERROR;
}

bool MainToolModule::SetupCommandLine(CommandLineManager& manager)
{
    manager.AddParameter(noEmailCommand,     "Doesn't send report via email.");
    manager.AddParameter(noShutdownCommand,  "Doesn't shutdown client neither server after running.");
    manager.AddParameter(onlyOneJobCommand,  "[JOBNAME] Only runs job JOBNAME.");
    manager.AddParameter(confFileCommand,    "[CONFIGURATION FILE] Specifies which configuration file to use.");

    manager.EnableHelpCommand();
    manager.EnableVersionCommand("Task Manager", version, "Mickael C. Guimaraes", "2014-2017");

    return (manager.HandleUnknownParameters() || manager.HandleVersionCommand() ||
            manager.HandleHelpCommand());
}

string MainToolModule::GetConfigurationFile(const string& commandLineFile)
{
    return (commandLineFile != "") ? commandLineFile : DEFAULT_CONFIGURATION_FILE;
}

void MainToolModule::ShowErrors(vector<string>& errorMessages)
{
    if (errorMessages.size() == 0)
        return;

    vector<string>::const_iterator it=errorMessages.begin();
    for (; it!=errorMessages.end(); it++)
        cout << *it << endl;
    cout << endl;
}

bool MainToolModule::SetupShutdownOptions(const bool isConfigShutdownEnabled,
                                          const bool isCommandLineShutdownCanceled,
                                          ClientWorkManager* workList)
{
    bool localShutdown = isConfigShutdownEnabled;
    if (isCommandLineShutdownCanceled)
    {
        localShutdown = false;
        workList->RemoveJob("Shutdown");
    }
    return localShutdown;
}

void MainToolModule::SetupSingleJobOption(ClientWorkManager* workList,
                                          const CommandLineManager& commandLine)
{
    string singleJob = commandLine.GetParameterValue(onlyOneJobCommand);
    if (singleJob != "")
        workList->RemoveAllButJobs(singleJob);
}

AbstractReportCreator* MainToolModule::RunWorkList(ClientWorkManager* workList,
                                                   const ServerConfiguration& configuration,
                                                   const vector<string>& configurationErrors)
{
    WorkResultData* workResult = workList->RunWorkList();
    AbstractReportCreator* reportCreator = configuration.GetReportCreator();
    reportCreator->Generate(workResult, configurationErrors, version);
    delete workResult;

    return reportCreator;
}

void MainToolModule::DispatchReport(AbstractReportCreator* reportCreator,
                    const ServerConfiguration& configuration,
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
        const string fallbackName = (dispatcher != fallbackDispatcher) ?
                    fallbackDispatcher->GetName() :
                    string("");

        reportCreator->UpdateWithDispatchError(dispatcher->GetName(), fallbackName);
    }
    return ok;
}

bool MainToolModule::RunLocalShutdown(const bool isLocalShutdownEnabled)
{
    if (isLocalShutdownEnabled == false)
        return true;

    ConsoleJob finalShutdown("/sbin/poweroff");
    JobStatus* status = finalShutdown.Run();
    const bool shutdownError = (status->GetCode() != JobStatus::OK);
    if (shutdownError)
        cout << "Local shutdown failed : " << status->GetDescription() << endl;

    delete status;
    return !shutdownError;
}

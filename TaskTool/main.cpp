#include <fstream>
#include <iostream>
#include <string.h>

#include "client.h"
#include "clientworkmanager.h"
#include "commandlinemanager.h"
#include "configuration.h"
#include "consolejob.h"
#include "consolereportdispatcher.h"
#include "emaildispatcherfactory.h"
#include "filetools.h"
#include "SelfIdentity.h"

//------------------------------------------------------------------------------

using namespace std;

static const string PROGRAM_VERSION = "0.713";
static const string DEFAULT_CONFIGURATION_FILE = "configuration.txt";

static const string noEmailCommand = "noemail";
static const string noShutdownCommand = "noshutdown";
static const string onlyOneJobCommand = "onlyjob";
static const string confFileCommand = "conffile";

//------------------------------------------------------------------------------

bool SetupCommandLine(CommandLineManager& manager);

string GetConfigurationFile(const string& commandLineFile);

void ShowErrors(vector<string> &errorMessages);

bool SetupShutdownOptions(const bool isConfigShutdownEnabled, const bool isCommandLineShutdownCanceled,
                          ClientWorkManager* workList);

AbstractReportCreator* RunWorkList(ClientWorkManager* workList, const Configuration& configuration);

void DispatchReport(AbstractReportCreator* reportCreator,
                    const Configuration& configuration,
                    const CommandLineManager& commandLine);

AbstractReportDispatcher* ReplaceDispatcherIfEmail(AbstractReportDispatcher* input,
                                                   const Configuration& configuration);

bool RunLocalShutdown(const bool isLocalShutdownEnabled);

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    const int NO_ERROR              = 0;
    const int CONFIGURATION_ERROR   = 1;
    const int OTHER_ERROR           = 2;

    CommandLineManager commandLine(argc, argv);
    bool shouldReturn = SetupCommandLine(commandLine);
    if (shouldReturn)
        return NO_ERROR;

    string configurationFile = GetConfigurationFile(commandLine.GetParameterValue(confFileCommand));
    if (FileTools::FileExists(configurationFile) == false)
    {
        cout << "Error : file " << configurationFile << " could not be opened." << endl;
        return CONFIGURATION_ERROR;
    }

    Configuration configuration;
    vector<string> configurationErrors;
    bool configurationIsUsable = configuration.LoadFromFile(configurationFile, configurationErrors);
    ShowErrors(configurationErrors);
    if (configurationIsUsable == false)
    {
        cout << "Unrecoverable errors while trying to read configuration file. Exiting." << endl;
        return CONFIGURATION_ERROR;
    }

    ClientWorkManager* workList = configuration.BuildTimedWorkList();

    bool localShutdown = SetupShutdownOptions(configuration.GetLocalShutdown(),
                                              commandLine.HasParameter(noShutdownCommand),
                                              workList);

    string singleJob = commandLine.GetParameterValue(onlyOneJobCommand);
    if (singleJob != "")
        workList->RemoveAllButJobs(singleJob);

    AbstractReportCreator* reportCreator = RunWorkList(workList, configuration);
    DispatchReport(reportCreator, configuration, commandLine);
    delete reportCreator;

    bool ok = RunLocalShutdown(localShutdown);
    return (ok) ? NO_ERROR : OTHER_ERROR;
    //return 0;
}

//------------------------------------------------------------------------------

bool SetupCommandLine(CommandLineManager& manager)
{
    manager.AddParameter(noEmailCommand,     "Doesn't send report via email.");
    manager.AddParameter(noShutdownCommand,  "Doesn't shutdown client neither server after running.");
    manager.AddParameter(onlyOneJobCommand,  "[JOBNAME] Only runs job JOBNAME.");
    manager.AddParameter(confFileCommand,    "[CONFIGURATION FILE] Specifies which configuration file to use.");

    manager.EnableHelpCommand();
    manager.EnableVersionCommand("Task Manager", PROGRAM_VERSION, "Mickael C. Guimaraes", "2014-2017");

    return (manager.HandleUnknownParameters() || manager.HandleVersionCommand() ||
            manager.HandleHelpCommand());
}

string GetConfigurationFile(const string& commandLineFile)
{
    return (commandLineFile != "") ? commandLineFile : DEFAULT_CONFIGURATION_FILE;
}

void ShowErrors(vector<string>& errorMessages)
{
    if (errorMessages.size() == 0)
        return;

    vector<string>::const_iterator it=errorMessages.begin();
    for (; it!=errorMessages.end(); it++)
        cout << *it << endl;
    cout << endl;
}

bool SetupShutdownOptions(const bool isConfigShutdownEnabled, const bool isCommandLineShutdownCanceled,
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

AbstractReportCreator* RunWorkList(ClientWorkManager* workList, const Configuration& configuration)
{
    WorkResultData* workResult = workList->RunWorkList();
    AbstractReportCreator* reportCreator = configuration.GetReportCreator();
    reportCreator->Generate(workResult, PROGRAM_VERSION);
    delete workResult;

    return reportCreator;
}

void DispatchReport(AbstractReportCreator* reportCreator,
                    const Configuration& configuration,
                    const CommandLineManager& commandLine)
{
    AbstractReportDispatcher* reportDispatcher = configuration.CreateReportDispatcher(
                commandLine.HasParameter(noEmailCommand)
                );
    reportDispatcher = ReplaceDispatcherIfEmail(reportDispatcher, configuration);
    bool ok = reportDispatcher->Dispatch(reportCreator);
    if (!ok)
    {
        ConsoleReportDispatcher fallbackDispatcher;
        fallbackDispatcher.Initialize(&configuration);

        cout << reportDispatcher->GetName() << "failed. ";
        cout << "Using " << fallbackDispatcher.GetName() << endl;

        fallbackDispatcher.Dispatch(reportCreator);
    }
}

AbstractReportDispatcher* ReplaceDispatcherIfEmail(AbstractReportDispatcher* input,
                                                   const Configuration& configuration)
{
    // For linking reasons, email dispatcher creation is being done in tool, not in
    // tasklib. Here is the workaround to get the final dispatcher : replace the dummy
    // one returned from configuration to the real one linked in tool.

    if (input->GetName() == "Dummy Email")
    {
        delete input;
        AbstractReportDispatcher* trueDispatcher = EmailDispatcherFactory::Create();
        trueDispatcher->Initialize(&configuration);
        return trueDispatcher;
    }
    else
        return input;
}

bool RunLocalShutdown(const bool isLocalShutdownEnabled)
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

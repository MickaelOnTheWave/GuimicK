#include <iostream>
#include <string.h>

#include "client.h"
#include "clientworkmanager.h"
#include "commandlinemanager.h"
#include "consolejob.h"
#include "configuration.h"
#include "emaildispatcherfactory.h"
#include "emailreportdispatcher.h"
#include "SelfIdentity.h"

#include <fstream>

using namespace std;

static const string PROGRAM_VERSION = "0.710";
static const string DEFAULT_CONFIGURATION_FILE = "configuration.txt";

void ShowErrors(vector<string> &errorMessages);
void SendReportByEmail(AbstractReportCreator* reportCreator, SelfIdentity* self, const Configuration& configuration);

int main(int argc, char* argv[])
{
    // TODO : clean this code. Main should be a lot smaller.

    const int NO_ERROR              = 0;
    const int CONFIGURATION_ERROR   = 1;

    CommandLineManager commandLine(argc, argv);

    commandLine.AddParameter("noemail",     "Doesn't send report via email.");
    commandLine.AddParameter("noshutdown",  "Doesn't shutdown client neither server after running.");
    commandLine.AddParameter("onlybackup",  "Only performs backup job.");
    commandLine.AddParameter("conffile",    "[CONFIGURATION FILE] Specifies which configuration file to use.");

    commandLine.EnableHelpCommand();
    commandLine.EnableVersionCommand("Task Manager", PROGRAM_VERSION, "Mickael C. Guimaraes", "2014-2017");

    if (commandLine.HandleUnknownParameters())
        return NO_ERROR;
    else if (commandLine.HandleVersionCommand())
        return NO_ERROR;
    else if (commandLine.HandleHelpCommand())
        return NO_ERROR;

    string configurationFile(DEFAULT_CONFIGURATION_FILE);
    if (commandLine.HasParameter("conffile"))
    {
        string newConfigurationFile = commandLine.GetParameterValue("conffile");
        ifstream file(newConfigurationFile.c_str());
        if (file.is_open())
            configurationFile = newConfigurationFile;
        else
        {
            cout << "Error : file " << newConfigurationFile << " could not be opened." << endl;
            return CONFIGURATION_ERROR;
        }
    }

    Configuration configuration;
    vector<string> errors;
    bool configurationIsUsable = configuration.LoadFromFile(configurationFile, errors);
    ShowErrors(errors);
    if (configurationIsUsable == false)
    {
        cout << "Unrecoverable errors while trying to read configuration file. Exiting." << endl;
        return CONFIGURATION_ERROR;
    }

    SelfIdentity* selfIdentity = configuration.GetAgent();

    ClientWorkManager* workList = configuration.BuildTimedWorkList();
    AbstractReportCreator* reportCreator = configuration.GetReportCreator();

    // Use configuration and consider command line as priority when specified
    bool sendReportByEmail = configuration.GetSendReportByEmail();
    if (commandLine.HasParameter("noemail"))
        sendReportByEmail = false;

    bool localShutdown = configuration.GetLocalShutdown();
    if (commandLine.HasParameter("noshutdown"))
    {
        localShutdown = false;
        workList->RemoveJob("Shutdown");
    }

    bool onlyBackup = commandLine.HasParameter("onlybackup");
    if (onlyBackup)
        workList->RemoveAllButJobs("RSnapshot Backup");

    WorkResultData* workResult = workList->RunWorkList();

    reportCreator->Generate(workResult, PROGRAM_VERSION);
    string reportData = reportCreator->GetReportContent();
    delete workResult;

    if (sendReportByEmail)
        SendReportByEmail(reportCreator, selfIdentity, configuration);
    else
        cout << reportData << endl;

    delete reportCreator;

    if (localShutdown)
    {
        ConsoleJob finalShutdown("/sbin/poweroff");
        JobStatus* status = finalShutdown.Run();
        if (status->GetCode() != JobStatus::OK)
            cout << "Local shutdown failed : " << status->GetDescription() << endl;
    }

    return NO_ERROR;
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

void SendReportByEmail(AbstractReportCreator* reportCreator, SelfIdentity* self, const Configuration& configuration)
{
    const string reportData = reportCreator->GetReportContent();
    EmailReportDispatcher* emailDispatcher = EmailDispatcherFactory::Create();
    emailDispatcher->Initialize(self, configuration);

    bool emailOk = emailDispatcher->Dispatch(reportCreator);
    if (!emailOk)
    {
        cout << "Email failed" << endl;
        cout << "Maintenance report : " << endl << reportData << endl;
    }

    delete emailDispatcher;
}

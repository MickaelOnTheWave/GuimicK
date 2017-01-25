#include <iostream>
#include <string.h>

#include "client.h"
#include "clientworkmanager.h"
#include "consolejob.h"
#include "dummyjob.h"
#include "curlconsoleemailsender.h"
#include "profiledjob.h"
#include "sleepjob.h"
#include "consolewakejob.h"
#include "changescreensaverjob.h"
#include "shutdownjob.h"
#include "rsnapshotbackupjob.h"
#include "clamavjob.h"
#include "configuration.h"
#include "SelfIdentity.h"
#include "textreportcreator.h"

#include <commandlinemanager.h>

#include <fstream>

using namespace std;

static const string PROGRAM_VERSION = "0.56";
static const string DEFAULT_CONFIGURATION_FILE = "configuration.txt";

void ShowErrors(list<string> &errorMessages);

int main(int argc, char* argv[])
{
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
    list<string> errors;
    bool configurationIsUsable = configuration.LoadFromFile(configurationFile, errors);
    ShowErrors(errors);
    if (configurationIsUsable == false)
    {
        cout << "Unrecoverable errors while trying to read configuration file. Exiting." << endl;
        return CONFIGURATION_ERROR;
    }


    SelfIdentity* selfIdentity = configuration.GetSelfIdentity();

    ClientWorkManager* workList = configuration.BuildWorkList();
    AbstractReportCreator* reportCreator = configuration.CreateReportObject();

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

    string reportData = reportCreator->Generate(workResult, PROGRAM_VERSION);
    delete workResult;

    CurlConsoleEmailSender sender;
    sender.SetSenderData(
                selfIdentity->name,
                selfIdentity->email,
                selfIdentity->emailPassword,
                selfIdentity->emailSmtpServer,
                selfIdentity->emailSmtpPort,
                selfIdentity->emailUseSsl
    );


    if (sendReportByEmail)
    {
        vector<string> externalFiles;
        vector<pair<string,string> > fileBuffers;
        reportCreator->GetAssociatedFiles(externalFiles, fileBuffers);
        bool emailOk = sender.Send(configuration.IsHtmlReport(), configuration.GetMasterEmail(), "", "", "Maintenance report",
                                            reportData, externalFiles, fileBuffers);
        if (!emailOk)
        {
            cout << "Email failed" << endl;
            cout << "Maintenance report : " << endl << reportData << endl;
        }
    }
    else
        cout << reportData << endl;

    delete reportCreator;

    if (localShutdown)
    {
        ConsoleJob finalShutdown("Local Shutdown", "/sbin/poweroff", "");
        JobStatus* status = finalShutdown.Run();
        if (status->GetCode() != JobStatus::OK)
            cout << "Local shutdown failed : " << status->GetDescription() << endl;
    }

    return NO_ERROR;
}

void ShowErrors(list<string>& errorMessages)
{
    if (errorMessages.size() == 0)
        return;

    list<string>::const_iterator it=errorMessages.begin();
    list<string>::const_iterator end=errorMessages.end();
    for (; it!=end; it++)
        cout << *it << endl;
    cout << endl;
}

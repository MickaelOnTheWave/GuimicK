#include "configuration.h"

#include <vector>

#include "consolejob.h"
#include "consolereportdispatcher.h"
#include "configurationparser.h"
#include "dummyemailreportdispatcher.h"
#include "filereportdispatcher.h"
#include "htmlreportcreator.h"
#include "profiledjob.h"
#include "tools.h"

#include "changescreensaverjobconfiguration.h"
#include "clamavjobconfiguration.h"
#include "copyfsbackupjobconfigurations.h"
#include "diskspacecheckjobconfiguration.h"
#include "gitbackupjobconfiguration.h"
#include "gitfsbackupjobconfiguration.h"
#include "rsnapshotbackupjobconfiguration.h"
#include "shutdownjobconfiguration.h"
#include "sshconsolejobconfiguration.h"
#include "userconsolejobconfiguration.h"
#include "wakejobconfiguration.h"

using namespace std;

Configuration::Configuration()
    : client(NULL), self(NULL), reportCreator(NULL), masterEmail("")
{
    reportDispatching = "email";
	shutdown = true;
    hasFatalError = false;

    FillSupportedJobsList();
}

Configuration::~Configuration()
{
    delete client;
	delete self;

    vector<AbstractJobConfiguration*>::iterator it=supportedJobs.begin();
    for (; it!=supportedJobs.end(); ++it)
        delete *it;
    supportedJobs.clear();
}

bool Configuration::LoadFromFile(const string &fileName, std::vector<string> &errorMessages)
{
	errorMessages.clear();
	jobList.clear();
    hasFatalError = false;

	ConfigurationParser parser;
	bool result = parser.ParseFile(fileName, errorMessages);
	if (!result)
		return false;

    FillRootObjects(parser.objectList, errorMessages);
    FillGlobalProperties(parser.anonymousObject, errorMessages);

    return IsConfigurationConsistent(errorMessages);
}

AbstractJob* Configuration::CreateJobFromObject(ConfigurationObject* object,
                                                std::vector<string> &errorMessages)
{
    AbstractJobConfiguration* relatedConfiguration = GetJobConfiguration(object->name);
    if (relatedConfiguration == NULL)
    {
        string errorMessage = "Warning : unknown job \"";
        errorMessage += object->name + "\". Ignoring...";
        errorMessages.push_back(errorMessage);
        return NULL;
    }
    else
        return relatedConfiguration->CreateConfiguredJob(object, errorMessages);
}

bool Configuration::CreateClient(ConfigurationObject *confObject, vector<string> &errorMessages)
{
    if (client != NULL)
    {
        errorMessages.push_back("Warning : only one client is supported for now. "
                                "Redefining default client");
        delete client;
    }

    client = new Client();

    // TODO : either put all this into client or into a separate client config class.

    const string clientName = confObject->GetProperty("Name");
    if (clientName == "")
    {
        errorMessages.push_back("Error : Client without name");
        return false;
    }
    client->SetName(clientName);

    bool ok = AreClientPropertiesConsistent(confObject, errorMessages);
    if (ok == false)
        return false;

    const string remoteJobList = confObject->GetProperty("remoteJobList");
    if (remoteJobList == "true")
        ok = FillJobListRemotely(client, errorMessages);
    else
        ok = FillJobListLocally(confObject->GetObject("JobList"), errorMessages);

    if (!ok)
        return false;

    // TODO : refactor this so that this warning is not shown if there is no job list
    // (redundancy)
    if (jobList.empty())
        errorMessages.push_back("Warning : client has an empty job list");

    return true;
}

void Configuration::CreateAgent(ConfigurationObject *confObject, vector<string> &errorMessages)
{
    if (self != NULL)
    {
        errorMessages.push_back("Warning : redefining SelfIdentity object");
        delete self;
    }

    self = new SelfIdentity(confObject, errorMessages);

    // TODO : think about the proper place for that
    ConfigurationObject* pathsObject = confObject->GetObject("DefaultBinPaths");
    if (pathsObject)
    {
        map<string, string>::iterator itPath = pathsObject->propertyList.begin();
        map<string, string>::iterator endPath = pathsObject->propertyList.end();
        for (; itPath != endPath; itPath++)
        {
            pair<string, string> currentPathPair = *itPath;
            ConsoleJob::AddAppSearchPath(currentPathPair.second);
        }
    }
}

void Configuration::CreateReport(ConfigurationObject *confObject, vector<string> &errorMessages)
{
    string reportType = confObject->GetFirstProperty("type", "param0");
    reportCreator = CreateReportObject(reportType);
    if (reportCreator == NULL)
    {
        reportCreator = new TextReportCreator();
        string message = "Warning : unsupported \"";
        message += reportType + "\" report type. Defaulting to text";
        errorMessages.push_back(message);
    }

    string useProfiling = confObject->GetFirstProperty("timed", "param1");
    reportCreator->UseProfileColumn(useProfiling != "false");

    string cssFile = confObject->GetProperty("css");
    if (cssFile != "")
    {
        HtmlReportCreator* htmlReportCreator = dynamic_cast<HtmlReportCreator*>(reportCreator);
        if (htmlReportCreator)
            htmlReportCreator->SetCssFile(cssFile);
    }
}

bool Configuration::GetBooleanValue(const string &strValue, vector<string> &errorMessages) const
{
	if (strValue == "true")
		return true;
	else if (strValue == "false")
		return false;

	std::string error("Warning : ");
    error += strValue + " is not a valid boolean value. Defaulting to false";
	errorMessages.push_back(error);
    return false;
}

AbstractJobConfiguration *Configuration::GetJobConfiguration(const string &jobTab)
{
    vector<AbstractJobConfiguration*>::iterator it = supportedJobs.begin();
    for (; it != supportedJobs.end(); ++it)
    {
        if ((*it)->GetTagName() == jobTab)
            return *it;
    }
    return NULL;
}

bool Configuration::AreClientPropertiesConsistent(ConfigurationObject *object,
                                                  std::vector<string> &errorMessages)
{
    map<string, string>::iterator itProp = object->propertyList.begin();
    map<string, string>::iterator endProp = object->propertyList.end();
    for (; itProp != endProp; itProp++)
    {
        pair<string, string> currentProp = *itProp;

        // Name already handled
        if (currentProp.first == "Name")
            continue;

        client->AddProperty(currentProp.first, currentProp.second);
    }

    if (client->GetProperty("ip") == "")
    {
        errorMessages.push_back("Error : Client without IP address");
        return false;
    }
    else if (client->GetProperty("sshuser") == "")
    {
        errorMessages.push_back("Error : Client without Ssh user");
        return false;
    }
    else
        return true;
}

bool Configuration::FillJobListLocally(ConfigurationObject* jobListObj,
                                       std::vector<std::string> &errorMessages)
{
    if (jobListObj == NULL)
        errorMessages.push_back("Warning : client without job list");
    else
    {
        list<ConfigurationObject*>::iterator itJobs = jobListObj->objectList.begin();
        list<ConfigurationObject*>::iterator endJobs = jobListObj->objectList.end();
        for (; itJobs != endJobs; itJobs++)
        {
            AbstractJob* parsedJob = CreateJobFromObject(*itJobs, errorMessages);
            if (parsedJob)
                jobList.push_back(parsedJob);
        }
    }

    return true;
}

bool Configuration::FillJobListRemotely(Client* client, std::vector<string> &errorMessages)
{
    const string tempClientFile = "tempClientConfiguration.conf";
    const string defaultRemoteConfigurationFile = "~/.taskmanager";

    string remoteConfigurationFile = client->GetProperty("remoteConfigurationFile");
    if (remoteConfigurationFile == "")
        remoteConfigurationFile = defaultRemoteConfigurationFile;

    // TODO refactor this : Configuration parser should be made like other parsers,
    // can parse from file or buffer. And here it should parse buffer!
    bool clientFileOk = CopyClientFile(client, remoteConfigurationFile, tempClientFile,
                                       errorMessages);
    if (!clientFileOk)
        return false;

    ConfigurationParser parser;
    bool result = parser.ParseFile(tempClientFile, errorMessages);
    if (!result)
        return false;

    FillRemoteClientObjects(parser.objectList, errorMessages);

    ConsoleJob::Run("rm", string("-f ") + tempClientFile);
    return true;
}

bool Configuration::CopyClientFile(Client* client,
                                   const string &source, const string &destination,
                                   vector<string> &errorMessages)
{
    const string host = client->GetProperty("ip");
    if (!Tools::IsComputerAlive(host))
    {
        errorMessages.push_back("Error : Client not available");
        return false;
    }

    string scpParams = "-o \"PasswordAuthentication no\" ";
    scpParams += client->GetProperty("sshuser") + "@" + host + ":";
    scpParams += source + " " + destination;
    ConsoleJob copyJob("scp", scpParams);

    copyJob.RunWithoutStatus();

    if (!copyJob.IsRunOk())
    {
        const string errorMessage = CreateScpErrorMessage(copyJob.GetCommandOutput());
        errorMessages.push_back(errorMessage);
        return false;
    }
    else
        return true;
}

void Configuration::FillRemoteClientObjects(const list<ConfigurationObject*> &objectList,
                                            vector<string> &errorMessages)
{
    if (objectList.size() > 1)
        errorMessages.push_back("Client configuration has unknown objects");
    else if (objectList.size() == 0)
        errorMessages.push_back("Client configuration is empty");
    else
        FillJobListLocally(objectList.front(), errorMessages);
}

string Configuration::CreateScpErrorMessage(const string &output) const
{
    if (output.find("Permission denied") != string::npos)
    {
        return "Error : Client requires password";
    }
    else if (output.find("No such file or directory") != string::npos)
        return "Error : Client configuration file missing";
    else
        return "Error trying to access Client configuration";
}

ClientWorkManager *Configuration::BuildTimedWorkList() const
{
	ClientWorkManager* workManager = new ClientWorkManager(client);

	list<AbstractJob*>::const_iterator it = jobList.begin();
	list<AbstractJob*>::const_iterator end = jobList.end();
	for (; it!=end; it++)
        workManager->AddJob(new ProfiledJob((*it)->Clone()));

    return workManager;
}

ClientWorkManager *Configuration::BuildSimpleWorkList() const
{
    ClientWorkManager* workManager = new ClientWorkManager(client->Clone());

    list<AbstractJob*>::const_iterator it = jobList.begin();
    list<AbstractJob*>::const_iterator end = jobList.end();
    for (; it!=end; it++)
        workManager->AddJob((*it)->Clone());

    return workManager;
}

AbstractReportCreator *Configuration::GetReportCreator() const
{
    return reportCreator;
}

AbstractReportDispatcher *Configuration::CreateReportDispatcher(
        const bool commandLinePreventsEmail) const
{
    AbstractReportDispatcher* dispatcher = NULL;

    if (reportDispatching == "email" && !commandLinePreventsEmail)
        dispatcher = new DummyEmailReportDispatcher();
    else if (reportDispatching == "file")
        dispatcher = new FileReportDispatcher();
    else // reportDispatching == "console"
        dispatcher = new ConsoleReportDispatcher();

    dispatcher->Initialize(this);
    return dispatcher;
}

AbstractReportCreator *Configuration::CreateReportObject(const string& type) const
{
    if (type == "text")
		return new TextReportCreator();
    else if (type == "html")
        return new HtmlReportCreator();
	else
        return NULL;
}

const SelfIdentity *Configuration::GetAgent() const
{
    return self;
}

Client *Configuration::GetClient()
{
    return client;
}

string Configuration::GetMasterEmail() const
{
	return masterEmail;
}

bool Configuration::GetLocalShutdown() const
{
	return shutdown;
}

std::string Configuration::GetReportDispatching() const
{
    return reportDispatching;
}

bool Configuration::IsReportHtml() const
{
    return (dynamic_cast<HtmlReportCreator*>(reportCreator));
}

bool Configuration::HasClient() const
{
    return (client != NULL);
}

void Configuration::FillSupportedJobsList()
{
    supportedJobs.push_back(new WakeJobConfiguration);
    supportedJobs.push_back(new ChangeScreensaverJobConfiguration);
    supportedJobs.push_back(new RsnapshotBackupJobConfiguration);
    supportedJobs.push_back(new ClamAvJobConfiguration);
    supportedJobs.push_back(new ShutdownJobConfiguration);
    supportedJobs.push_back(new UserConsoleJobConfiguration);
    supportedJobs.push_back(new SshConsoleJobConfiguration);
    supportedJobs.push_back(new GitBackupJobConfiguration);
    supportedJobs.push_back(new DiskSpaceCheckJobConfiguration);
    supportedJobs.push_back(new GitFsBackupJobConfiguration);
    supportedJobs.push_back(new RawCopyFsBackupJobConfiguration());
    supportedJobs.push_back(new RsyncCopyFsBackupJobConfiguration());
    supportedJobs.push_back(new ZipAndCopyFsBackupJobConfiguration());
}

void Configuration::FillRootObjects(const list<ConfigurationObject *> &objectList,
                                    vector<string> &errorMessages)
{
    list<ConfigurationObject*>::const_iterator it = objectList.begin();
    list<ConfigurationObject*>::const_iterator end = objectList.end();
    for (; it!=end; it++)
    {
        bool returnValue = true;
        ConfigurationObject* currentObject = *it;
        if (currentObject->name == "Client")
            returnValue = CreateClient(currentObject, errorMessages);
        else if (currentObject->name == "Agent")
            CreateAgent(currentObject, errorMessages);
        else if (currentObject->name == "Report")
            CreateReport(currentObject, errorMessages);
        else
        {
            string message = "Warning : unknown object \"";
            message += currentObject->name + "\"";
            errorMessages.push_back(message);
        }

        if (returnValue == false)
            hasFatalError = true;
    }
}

void Configuration::FillGlobalProperties(ConfigurationObject *object,
                                         vector<string> &errorMessages)
{
    if (!object)
    {
        errorMessages.push_back("Warning : no global properties defined!");
        return;
    }

    map<string, string>::iterator itProp = object->propertyList.begin();
    map<string, string>::iterator endProp = object->propertyList.end();
    for (; itProp != endProp; itProp++)
    {
        pair<string, string> currentProp = *itProp;
        if (currentProp.first == "MasterEmail")
            masterEmail = currentProp.second;
        else if (currentProp.first == "ReportDispatching")
            reportDispatching = currentProp.second;
        else if (currentProp.first == "ShutdownOnFinish")
            shutdown = GetBooleanValue(currentProp.second, errorMessages);
        else
        {
            string message = "Warning : unknown property \"";
            message += currentProp.first + "\"";
            errorMessages.push_back(message);
        }
    }
}

bool Configuration::IsConfigurationConsistent(vector<string> &errorMessages)
{
    if (hasFatalError)
        return false;
    else if (self == NULL)
    {
        errorMessages.push_back("Error : missing Agent configuration");
        return false;
    }
    else if (client == NULL)
    {
        errorMessages.push_back("Error : missing Client");
        return false;
    }
    else if (reportCreator == NULL)
    {
        reportCreator = new TextReportCreator();
        errorMessages.push_back("Warning : missing Report configuration. Defaulting to text");
        return true;
    }
    else if (reportDispatching != "email" && reportDispatching != "file" &&
             reportDispatching != "console")
    {
        stringstream message;
        message << "Warning : unknown " << reportDispatching << " dispatching.";
        message << "Defaulting to console.";
        errorMessages.push_back(message.str());
        return true;
    }
    else if (reportDispatching == "email" && !IsEmailDataComplete())
    {
        errorMessages.push_back("Error : missing data for email sending. Defaulting to console.");
        return true;
    }
    else
        return true;
}

bool Configuration::IsEmailDataComplete() const
{
    return (self->HasValidEmailData() && masterEmail != "");
}

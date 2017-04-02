#include "configuration.h"

#include <stdlib.h>
#include <vector>

#include "configurationparser.h"
#include "htmlreportcreator.h"

#include "changescreensaverjob.h"
#include "clamavjob.h"
#include "profiledjob.h"
#include "rsnapshotsmartcreator.h"
#include "userconsolejob.h"

using namespace std;

Configuration::Configuration()
    : client(NULL), self(NULL), reportCreator(NULL), masterEmail("")
{
	emailReport = true;
	shutdown = true;
}

Configuration::~Configuration()
{
    delete client;
	delete self;
}

bool Configuration::LoadFromFile(const string &fileName, std::vector<string> &errorMessages)
{
	errorMessages.clear();
	jobList.clear();

	ConfigurationParser parser;
	bool result = parser.ParseFile(fileName, errorMessages);
	if (!result)
		return false;

    FillRootObjects(parser.objectList, errorMessages);
    FillGlobalProperties(parser.anonymousObject, errorMessages);

    return IsConfigurationConsistent(errorMessages);
}

AbstractJob* Configuration::CreateJobFromObject(ConfigurationObject* object)
{
	if (object->name == "Wake")
        return CreateWakeJobFromObject(object);
	else if (object->name == "ChangeScreenSaver")
        return CreateChangeScreensaverJobFromObject(object);
    else if (object->name == "RsnapshotBackup")
        return CreateRsnapshotBackupJob(object);
	else if (object->name == "ClamAv")
		return new ClamAvJob();
	else if (object->name == "Shutdown")
        return CreateShutdownJobFromObject(object);
    else if (object->name == "Console")
        return InitializeUserConsoleJobFromObject(object);
    else if (object->name == "SshConsole")
        return InitializeSshConsoleJobFromObject(object);
    else if (object->name == "GitBackup")
        return CreateGitBackupJob(object);
    else if (object->name == "DiskSpaceCheck")
        return CreateDiskSpaceCheckJob(object);
	else
        return NULL;
}

UserConsoleJob *Configuration::InitializeUserConsoleJobFromObject(ConfigurationObject *object) const
{
    string title =          object->GetFirstProperty("title", "param0");
    string command =        object->GetFirstProperty("command", "param1");
    string rawReturnCode =  object->GetFirstProperty("returnCode","param2");
    string expectedOutput = object->propertyList["expectedOutput"];
    string outputFile     = object->propertyList["outputFileName"];
    string parserCommand  = object->propertyList["parserCommand"];

    UserConsoleJob* job = new UserConsoleJob();
    job->SetTitle(title);
    job->Initialize(command);

    if (object->propertyList["showDebugInformation"] != "")
        job->SetOutputDebugInformation(true);

    if (rawReturnCode != "")
    {
        int returnCode = 0;
        stringstream ss(rawReturnCode);
        ss >> returnCode;
        job->SetExpectedReturnCode(returnCode);
    }

    if (expectedOutput != "")
        job->SetExpectedOutput(expectedOutput);

    if (outputFile != "")
        job->SetOutputTofile(outputFile);
    else
        job->SetAttachOutput(true);

    if (parserCommand != "")
        job->SetMiniDescriptionParserCommand(parserCommand);

    return job;
}

SshConsoleJob *Configuration::InitializeSshConsoleJobFromObject(ConfigurationObject *object) const
{
    UserConsoleJob* remoteJob = InitializeUserConsoleJobFromObject(object);
    return new SshConsoleJob(remoteJob->GetName(), remoteJob);
}

WakeJob *Configuration::CreateWakeJobFromObject(ConfigurationObject *object) const
{
    WakeJob* job = new WakeJob();
    string param = object->propertyList["param0"];
    if (param == "showDebugInformation")
        job->SetOutputDebugInformation(true);
    return job;
}

LinuxShutdownJob *Configuration::CreateShutdownJobFromObject(ConfigurationObject *object) const
{
    LinuxShutdownJob* job = new LinuxShutdownJob();
    string param = object->propertyList["param0"];
    if (param == "showDebugInformation")
        job->SetOutputDebugInformation(true);
    return job;
}

ChangeScreensaverJob *Configuration::CreateChangeScreensaverJobFromObject(ConfigurationObject *object) const
{
    int time = 600;
    string param = object->propertyList["param0"];
    if (param != "")
        time = atoi(param.c_str());
    return new ChangeScreensaverJob(time);
}

GitBackupJob *Configuration::CreateGitBackupJob(ConfigurationObject *object) const
{
    GitBackupJob* job = new GitBackupJob();
    list<ConfigurationObject*>::iterator it = object->objectList.begin();
    for (; it != object->objectList.end(); it++)
    {
        ConfigurationObject* currentObj = *it;

        if (currentObj->name != "Repository")
            continue;

        string source(currentObj->propertyList["source"]);
        string dest(currentObj->propertyList["dest"]);
        job->AddRepository(source, dest);
    }

    string target(object->propertyList["target"]);
    if (target == "local")
        job->SetTargetLocal();
    else
        job->SetTargetRemote();

    string writeLogsToFiles(object->propertyList["writeLogsToFiles"]);
    if (writeLogsToFiles == "true")
        job->SetWriteLogsToFiles(true);

    return job;
}

RsnapshotBackupJob *Configuration::CreateRsnapshotBackupJob(ConfigurationObject *object) const
{
    string repository = object->GetFirstProperty("repository", "param0");
    string configurationFile = object->GetFirstProperty("fullConfigurationFile", "param1");

    RsnapshotBackupJob* job = NULL;
    if (configurationFile != "")
        job = new RsnapshotBackupJob(repository, configurationFile);
    else
        job = CreateRsnapshotBackupJobFromCreator(object, repository);

    if (object->propertyList["showDebugInformation"] != "")
        job->SetOutputDebugInformation(true);
    if (object->propertyList["waitAfterRun"] != "")
        job->SetWaitAfterRun(true);
    return job;
}

LinuxFreeSpaceCheckJob *Configuration::CreateDiskSpaceCheckJob(ConfigurationObject *object) const
{
    const string drive = object->GetFirstProperty("drive", "param0");
    const string localTarget = object->GetFirstProperty("localTarget", "param1");

    LinuxFreeSpaceCheckJob* job = new LinuxFreeSpaceCheckJob(drive);
    if (localTarget == "false")
        job->SetTargetToRemote(true);

    return job;
}

RsnapshotBackupJob *Configuration::CreateRsnapshotBackupJobFromCreator(ConfigurationObject *object,
                                                                       const string &repository) const
{
    RsnapshotSmartCreator creator(repository);

    string templateFile = object->propertyList["templateConfigurationFile"];
    creator.SetTemplateConfigurationFile(templateFile);

    list<ConfigurationObject*>::iterator it = object->objectList.begin();
    for (; it != object->objectList.end(); it++)
    {
        ConfigurationObject* currentObj = *it;

        if (currentObj->name != "Folder")
            continue;

        string source(currentObj->propertyList["source"]);
        string dest(currentObj->propertyList["dest"]);
        creator.AddFolderToBackup(source, dest);
    }

    return creator.CreateConfiguredJob();
}

void Configuration::CreateClient(ConfigurationObject *confObject, vector<string> &errorMessages)
{
	if (client == NULL)
		client = new Client();

	client->SetName(confObject->propertyList["Name"]);

	map<string, string>::iterator itProp = confObject->propertyList.begin();
	map<string, string>::iterator endProp = confObject->propertyList.end();
	for (; itProp != endProp; itProp++)
	{
		pair<string, string> currentProp = *itProp;

		// Name already handled
		if (currentProp.first == "Name")
			continue;

		client->AddProperty(currentProp.first, currentProp.second);
	}

	ConfigurationObject* jobListObj = confObject->GetObject("JobList");
	if (jobListObj == NULL)
	{
		errorMessages.push_back("Warning : client without job list");
		return;
	}

	list<ConfigurationObject*>::iterator itJobs = jobListObj->objectList.begin();
	list<ConfigurationObject*>::iterator endJobs = jobListObj->objectList.end();
	for (; itJobs != endJobs; itJobs++)
	{
		AbstractJob* parsedJob = CreateJobFromObject(*itJobs);
		if (parsedJob == NULL)
		{
            string errorMessage = "Warning : unknown job \"";
            errorMessage += (*itJobs)->name + "\". Ignoring...";
            errorMessages.push_back(errorMessage);
			continue;
		}

        jobList.push_back(parsedJob);
	}

    if (jobList.empty())
        errorMessages.push_back("Warning : client has an empty job list");
}

void Configuration::CreateSelf(ConfigurationObject *confObject, vector<string> &errorMessages)
{
	if (self == NULL)
		self = new SelfIdentity();

	map<string, string>::iterator itProp = confObject->propertyList.begin();
	map<string, string>::iterator endProp = confObject->propertyList.end();
	for (; itProp != endProp; itProp++)
	{
		pair<string, string> currentProp = *itProp;

		if (currentProp.first == "Name")
			self->name = currentProp.second;
		else if (currentProp.first == "Email")
			self->email = currentProp.second;
		else if (currentProp.first == "Password")
			self->emailPassword = currentProp.second;
		else if (currentProp.first == "SmtpAddress")
			self->emailSmtpServer = currentProp.second;
		else if (currentProp.first == "SmtpPort")
			self->emailSmtpPort = atoi(currentProp.second.c_str());
		else if (currentProp.first == "UseSSL")
			self->emailUseSsl = GetBooleanValue(currentProp.second, errorMessages);
		else
            errorMessages.push_back("Warning : unknown property in Agent configuration");
	}

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

    string cssFile = confObject->propertyList["css"];
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

AbstractReportCreator *Configuration::CreateReportObject(const string& type) const
{
    if (type == "text")
		return new TextReportCreator();
    else if (type == "html")
        return new HtmlReportCreator();
	else
        return NULL;
}

SelfIdentity *Configuration::GetSelfIdentity()
{
	return self;
}

string Configuration::GetMasterEmail() const
{
	return masterEmail;
}

bool Configuration::GetLocalShutdown() const
{
	return shutdown;
}

bool Configuration::GetSendReportByEmail() const
{
    return emailReport;
}

bool Configuration::IsReportHtml() const
{
    return (dynamic_cast<HtmlReportCreator*>(reportCreator));
}

bool Configuration::HasClient() const
{
    return (client != NULL);
}

void Configuration::FillRootObjects(const list<ConfigurationObject *> &objectList,
                                    vector<string> &errorMessages)
{
    list<ConfigurationObject*>::const_iterator it = objectList.begin();
    list<ConfigurationObject*>::const_iterator end = objectList.end();
    for (; it!=end; it++)
    {
        ConfigurationObject* currentObject = *it;
        if (currentObject->name == "Client")
            CreateClient(currentObject, errorMessages);
        else if (currentObject->name == "Agent")
            CreateSelf(currentObject, errorMessages);
        else if (currentObject->name == "Report")
            CreateReport(currentObject, errorMessages);
        else
        {
            string message = "Warning : unknown object \"";
            message += currentObject->name + "\"";
            errorMessages.push_back(message);
        }
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
        else if (currentProp.first == "SendReportByEmail")
            emailReport = GetBooleanValue(currentProp.second, errorMessages);
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
    if (self == NULL)
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
    else if (emailReport && !IsEmailDataComplete())
    {
        errorMessages.push_back("Error : missing data for email sending");
        return false;
    }
    else
        return true;
}

bool Configuration::IsEmailDataComplete() const
{
    return (self->HasValidEmailData() && masterEmail != "");
}

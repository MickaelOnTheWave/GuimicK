#include "configuration.h"

#include <vector>

#include "configurationparser.h"
#include "htmlreportcreator.h"
#include "profiledjob.h"

#include "changescreensaverjobconfiguration.h"
#include "clamavjobconfiguration.h"
#include "diskspacecheckjobconfiguration.h"
#include "gitbackupjobconfiguration.h"
#include "rsnapshotbackupjobconfiguration.h"
#include "shutdownjobconfiguration.h"
#include "sshconsolejobconfiguration.h"
#include "userconsolejobconfiguration.h"
#include "wakejobconfiguration.h"

using namespace std;

Configuration::Configuration()
    : client(NULL), self(NULL), reportCreator(NULL), masterEmail("")
{
	emailReport = true;
	shutdown = true;

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
        AbstractJob* parsedJob = CreateJobFromObject(*itJobs, errorMessages);
        if (parsedJob)
            jobList.push_back(parsedJob);
	}

    if (jobList.empty())
        errorMessages.push_back("Warning : client has an empty job list");
}

void Configuration::CreateSelf(ConfigurationObject *confObject, vector<string> &errorMessages)
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

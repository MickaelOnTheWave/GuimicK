#include "serverconfiguration.h"

#include <vector>

#include "configurationparser.h"
#include "consolejob.h"
#include "consolereportdispatcher.h"
#include "dummyemailreportdispatcher.h"
#include "filereportdispatcher.h"
#include "htmlreportcreator.h"
#include "profiledjob.h"
#include "tools.h"

#include "changescreensaverjobconfiguration.h"
#include "clamavjobconfiguration.h"
#include "clientconfiguration.h"
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

string ServerConfiguration::MsgNoPassword = "Client requires password";
string ServerConfiguration::MsgNoConfigFile = "Client configuration file missing";
string ServerConfiguration::MsgClientConfigAccessError = "Error trying to access Client configuration";
string ServerConfiguration::MsgClientConfigUnknownObjects = "Client configuration has unknown objects";
string ServerConfiguration::MsgClientConfigEmpty = "Client configuration is empty";
string ServerConfiguration::MsgMissingClient = "missing Client";
string ServerConfiguration::MsgMissingAgent = "missing Agent configuration";
string ServerConfiguration::MsgRemoteOptionDeprecated = "Remote option deprecated";
string ServerConfiguration::MsgOneClientSupported = "only one client is supported for now. "
                                              "Redefining default client";

ServerConfiguration::ServerConfiguration()
    : TaskManagerConfiguration(),
      self(NULL), reportCreator(NULL), masterEmail("")
{
    reportDispatching = "email";
	shutdown = true;
}

ServerConfiguration::~ServerConfiguration()
{
   delete self;
}

ClientWorkManager* ServerConfiguration::BuildWorkList(const bool withProfiling) const
{
   ClientWorkManager* workManager = new ClientWorkManager(client->Clone());
   workManager->AddJobsFromClient(withProfiling);
   return workManager;
}


bool ServerConfiguration::CreateClient(ConfigurationObject *confObject, vector<string> &errorMessages)
{
    if (client != NULL)
    {
        errorMessages.push_back(CreateWarning(MsgOneClientSupported));
        delete client;
    }

    ClientConfiguration config;
    client = config.CreateConfiguredClient(confObject, errorMessages);
    return (client != NULL);
}

void ServerConfiguration::CreateAgent(ConfigurationObject *confObject, vector<string> &errorMessages)
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

void ServerConfiguration::CreateReport(ConfigurationObject *confObject, vector<string> &errorMessages)
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

bool ServerConfiguration::GetBooleanValue(const string &strValue, vector<string> &errorMessages) const
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

AbstractReportCreator *ServerConfiguration::GetReportCreator() const
{
    return reportCreator;
}

AbstractReportDispatcher *ServerConfiguration::CreateReportDispatcher(
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

AbstractReportCreator *ServerConfiguration::CreateReportObject(const string& type) const
{
    if (type == "text")
		return new TextReportCreator();
    else if (type == "html")
        return new HtmlReportCreator();
	else
        return NULL;
}

const SelfIdentity *ServerConfiguration::GetAgent() const
{
    return self;
}

string ServerConfiguration::GetMasterEmail() const
{
	return masterEmail;
}

bool ServerConfiguration::GetLocalShutdown() const
{
	return shutdown;
}

std::string ServerConfiguration::GetReportDispatching() const
{
    return reportDispatching;
}

bool ServerConfiguration::IsReportHtml() const
{
    return (dynamic_cast<HtmlReportCreator*>(reportCreator));
}

void ServerConfiguration::FillRootObjects(const list<ConfigurationObject *> &objectList,
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

void ServerConfiguration::FillGlobalProperties(ConfigurationObject *object,
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

bool ServerConfiguration::IsConfigurationConsistent(vector<string> &errorMessages)
{
    if (hasFatalError)
        return false;
    else if (self == NULL)
    {
        errorMessages.push_back(CreateError(MsgMissingAgent));
        return false;
    }
    else if (client == NULL)
    {
        errorMessages.push_back(CreateError(MsgMissingClient));
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

bool ServerConfiguration::IsEmailDataComplete() const
{
    return (self->HasValidEmailData() && masterEmail != "");
}

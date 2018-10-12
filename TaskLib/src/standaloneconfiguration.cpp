#include "standaloneconfiguration.h"

#include <fstream>
#include "configurationtools.h"
#include "consolereportdispatcher.h"
#include "dummyemailreportdispatcher.h"
#include "filereportdispatcher.h"
#include "htmlreportcreator.h"
#include "localclientconfiguration.h"
#include "profiledjob.h"
#include "textreportcreator.h"

using namespace std;

string StandaloneConfiguration::MsgMissingClient = "missing Client";
string StandaloneConfiguration::MsgMissingAgent = "missing Agent configuration";
string StandaloneConfiguration::MsgOneClientSupported = "only one client is supported for now. "
                                              "Redefining default client";


StandaloneConfiguration::StandaloneConfiguration()
   : AbstractTypeConfiguration(),
     client(NULL), reportCreator(NULL), self(NULL),
     reportType(""), cssFile(""),
     masterEmail(""), reportDispatching("email"), shutdown(true)
{
}

StandaloneConfiguration::~StandaloneConfiguration()
{
   delete self;
}

ConfigurationType StandaloneConfiguration::GetType() const
{
   return ConfigurationType::Standalone;
}

void StandaloneConfiguration::SaveToOpenedFile(ofstream& fileStream)
{
   SaveAgentToOpenedFile(fileStream);
   SaveClientToOpenedFile(fileStream);
   SaveReportOptionsToOpenedFile(fileStream);
   SaveGlobalPropertiesToOpenedFile(fileStream);
}

void StandaloneConfiguration::GetJobList(std::list<AbstractJob*>& _jobList)
{
   client->GetJobList(_jobList);
}

void StandaloneConfiguration::SetJobList(const std::vector<AbstractJob*>& _jobList)
{
   if (client == NULL)
      CreateDefaultClient();

   client->ClearJobList();

   vector<AbstractJob*>::const_iterator it=_jobList.begin();
   vector<AbstractJob*>::const_iterator end=_jobList.end();
   for (; it!=end; ++it)
      client->AddJob((*it)->Clone());
}

void StandaloneConfiguration::ClearJobList()
{
   client->ClearJobList();
}

ClientWorkManager* StandaloneConfiguration::BuildWorkList(const bool withProfiling) const
{
   ClientWorkManager* workManager = new ClientWorkManager(client->Clone());
   workManager->AddJobsFromClient(withProfiling);
   return workManager;
}

string StandaloneConfiguration::GetMasterEmail() const
{
   return masterEmail;
}

bool StandaloneConfiguration::GetLocalShutdown() const
{
   return shutdown;
}

std::string StandaloneConfiguration::GetReportDispatching() const
{
    return reportDispatching;
}

AbstractReportCreator *StandaloneConfiguration::GetReportCreator() const
{
    return reportCreator;
}

const SelfIdentity *StandaloneConfiguration::GetAgent() const
{
   return self;
}

void StandaloneConfiguration::SetAgent(SelfIdentity* agent)
{
   self = agent;
}

Client*StandaloneConfiguration::GetClient()
{
   return client;
}

AbstractReportDispatcher *StandaloneConfiguration::CreateReportDispatcher(
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

bool StandaloneConfiguration::IsReportHtml() const
{
    return (dynamic_cast<HtmlReportCreator*>(reportCreator));
}

void StandaloneConfiguration::FillRootObjects(const list<ConfigurationObject*>& objectList,
                                              vector<string>& errorMessages)
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

void StandaloneConfiguration::FillGlobalProperties(ConfigurationObject* object,
                                                   vector<string>& errorMessages)
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
       {
           shutdown = ConfigurationTools::GetBooleanValue(currentProp.second,
                                                          errorMessages);
       }
       else
       {
           string message = "Warning : unknown property \"";
           message += currentProp.first + "\"";
           errorMessages.push_back(message);
       }
   }
}

bool StandaloneConfiguration::IsConfigurationConsistent(vector<string>& errorMessages)
{
   if (hasFatalError)
       return false;
   else if (self == NULL)
   {
       errorMessages.push_back(ConfigurationTools::CreateError(MsgMissingAgent));
       return false;
   }
   else if (client == NULL)
   {
       errorMessages.push_back(ConfigurationTools::CreateError(MsgMissingClient));
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

void StandaloneConfiguration::SaveAgentToOpenedFile(ofstream& file)
{
   if (self == NULL)
      CreateDefaultSelfIdentity();

   file << "Agent" << endl;
   file << "{" << endl;
   ConfigurationTools::SaveValueToFile(file, "Name", self->name);
   if (self->HasValidEmailData())
   {
      ConfigurationTools::SaveValueToFile(file, "Email", self->email);
      ConfigurationTools::SaveValueToFile(file, "Password", self->emailPassword);
      ConfigurationTools::SaveValueToFile(file, "SmtpAddress", self->emailSmtpServer);
      ConfigurationTools::SaveValueToFile(file, "SmtpPort", self->emailSmtpPort);
      ConfigurationTools::SaveValueToFile(file, "UseSSL", self->emailUseSsl);
   }

   file << "}" << endl;
}

void StandaloneConfiguration::SaveClientToOpenedFile(ofstream& file)
{
   file << "Client" << endl;
   file << "{" << endl;
   ConfigurationTools::SaveValueToFile(file, "Name", "Client"); //TODO : put real name
   ConfigurationTools::SaveValueToFile(file, "showDebugInformation", "never");


   list<AbstractJob*> clientJobList;
   client->GetJobList(clientJobList);
   ConfigurationTools::SaveJobListToFile(file, clientJobList);

   file << "}" << endl;
}

void StandaloneConfiguration::SaveReportOptionsToOpenedFile(ofstream& file)
{
   file << "Report" << endl;
   file << "{" << endl;
   ConfigurationTools::SaveValueToFile(file, "type", reportType);
   ConfigurationTools::SaveValueToFile(file, "css", cssFile);
   file << "}" << endl;
}

void StandaloneConfiguration::SaveGlobalPropertiesToOpenedFile(ofstream& file)
{
   file << "MasterEmail = \"" << masterEmail << "\";" << endl;
   file << "ReportDispatching = \"" << reportDispatching << "\";" << endl;
   file << "ShutdownOnFinish = " << shutdown << ";" << endl;
}

bool StandaloneConfiguration::CreateClient(ConfigurationObject *confObject,
                                           vector<string> &errorMessages)
{
    if (client != NULL)
    {
        errorMessages.push_back(ConfigurationTools::CreateWarning(MsgOneClientSupported));
        delete client;
    }

    LocalClientConfiguration config;
    client = config.CreateConfiguredClient(confObject, errorMessages);
    return (client != NULL);
}

void StandaloneConfiguration::CreateAgent(ConfigurationObject *confObject,
                                          vector<string> &errorMessages)
{
    if (self != NULL)
    {
        errorMessages.push_back("Warning : redefining SelfIdentity object");
        delete self;
    }

    self = new SelfIdentity(confObject, errorMessages);
}

void StandaloneConfiguration::CreateReport(ConfigurationObject *confObject,
                                           vector<string> &errorMessages)
{
    reportType = confObject->GetFirstProperty("type", "param0");
    reportCreator = CreateReportObject(reportType);
    if (reportCreator == NULL)
    {
        string message = "Warning : unsupported \"";
        message += reportType + "\" report type. Defaulting to text";
        errorMessages.push_back(message);

        reportCreator = new TextReportCreator();
        reportType = "text";
    }

    string useProfiling = confObject->GetFirstProperty("timed", "param1");
    reportCreator->UseProfileColumn(useProfiling != "false");

    cssFile = confObject->GetProperty("css");
    if (cssFile != "")
    {
        HtmlReportCreator* htmlReportCreator = dynamic_cast<HtmlReportCreator*>(reportCreator);
        if (htmlReportCreator)
            htmlReportCreator->SetCssFile(cssFile);
    }
}

AbstractReportCreator *StandaloneConfiguration::CreateReportObject(const string& type) const
{
    if (type == "text")
      return new TextReportCreator();
    else if (type == "html")
        return new HtmlReportCreator();
   else
        return NULL;
}


bool StandaloneConfiguration::IsEmailDataComplete() const
{
   return (self->HasValidEmailData() && masterEmail != "");
}

void StandaloneConfiguration::CreateDefaultClient()
{
   client = new Client("Default Client");
}

void StandaloneConfiguration::CreateDefaultSelfIdentity()
{
   self = new SelfIdentity();
   self->name = "DefaultAgent";
}

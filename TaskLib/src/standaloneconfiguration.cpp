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

const wstring defaultClientName = L"Local Machine";
wstring StandaloneConfiguration::MsgMissingClient = L"missing Client";
wstring StandaloneConfiguration::MsgMissingAgent = L"missing Agent configuration";
wstring StandaloneConfiguration::MsgOneClientSupported = L"only one client is supported for now. "
                                              "Redefining default client";


StandaloneConfiguration::StandaloneConfiguration()
   : AbstractTypeConfiguration(),
     client(new Client(defaultClientName)), reportCreator(NULL), agent(new Agent()),
     reportType(L"html"), cssFile(L""),
     masterEmail(L""), reportDispatching(L"file"), shutdown(false)
{
}

StandaloneConfiguration::StandaloneConfiguration(const StandaloneConfiguration& other)
   : AbstractTypeConfiguration(other),
     reportType(other.reportType), cssFile(other.cssFile), masterEmail(other.masterEmail),
     reportDispatching(other.reportDispatching), shutdown(other.shutdown)
{
   client = new Client(*other.client);
   agent = new Agent(*other.agent);
   reportCreator = (other.reportCreator) ? other.reportCreator->Copy() : NULL;
}

StandaloneConfiguration::~StandaloneConfiguration()
{
   delete agent;
}

ConfigurationType StandaloneConfiguration::GetType() const
{
   return StandaloneConfigurationType;
}

AbstractTypeConfiguration* StandaloneConfiguration::Copy() const
{
   return new StandaloneConfiguration(*this);
}

void StandaloneConfiguration::SaveToOpenedFile(wofstream& fileStream)
{
   agent->SaveToOpenedFile(fileStream);
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

wstring StandaloneConfiguration::GetMasterEmail() const
{
   return masterEmail;
}

void StandaloneConfiguration::SetMasterEmail(const wstring& value)
{
   masterEmail = value;
}

bool StandaloneConfiguration::GetLocalShutdown() const
{
   return shutdown;
}

void StandaloneConfiguration::SetLocalShutdown(const bool value)
{
   shutdown = value;
}

std::wstring StandaloneConfiguration::GetReportDispatching() const
{
   return reportDispatching;
}

void StandaloneConfiguration::SetReportDispatching(const wstring& value)
{
   reportDispatching = value;
}

wstring StandaloneConfiguration::GetReportType() const
{
   return reportType;
}

void StandaloneConfiguration::SetReportType(const wstring& value)
{
   reportType = value;
   ChangeReportCreator();
}

AbstractReportCreator *StandaloneConfiguration::GetReportCreator() const
{
   return reportCreator;
}

wstring StandaloneConfiguration::GetReportCss() const
{
   return cssFile;
}

void StandaloneConfiguration::SetReportCss(const wstring& value)
{
   cssFile = value;
}

Agent *StandaloneConfiguration::GetAgent() const
{
   return agent;
}

void StandaloneConfiguration::SetAgent(Agent* _agent)
{
   agent = _agent;
}

Client*StandaloneConfiguration::GetClient()
{
   return client;
}

AbstractReportDispatcher *StandaloneConfiguration::CreateReportDispatcher(
        const bool commandLinePreventsEmail) const
{
    AbstractReportDispatcher* dispatcher = NULL;

    if (reportDispatching == L"email" && !commandLinePreventsEmail)
        dispatcher = new DummyEmailReportDispatcher();
    else if (reportDispatching == L"file")
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
                                              vector<wstring>& errorMessages)
{
   delete client;
   delete agent;
   client = NULL;
   agent = NULL;

   list<ConfigurationObject*>::const_iterator it = objectList.begin();
   list<ConfigurationObject*>::const_iterator end = objectList.end();
   for (; it!=end; it++)
   {
       bool returnValue = true;
       ConfigurationObject* currentObject = *it;
       if (currentObject->GetName() == L"Client")
           returnValue = CreateClient(currentObject, errorMessages);
       else if (currentObject->GetName() == L"Agent")
           CreateAgent(currentObject, errorMessages);
       else if (currentObject->GetName() == L"Report")
           CreateReport(currentObject, errorMessages);
       else
       {
           wstring message = L"Warning : unknown object \"";
           message += currentObject->GetName() + L"\"";
           errorMessages.push_back(message);
       }

       if (returnValue == false)
           hasFatalError = true;
   }
}

void StandaloneConfiguration::FillGlobalProperties(ConfigurationObject* object,
                                                   vector<wstring>& errorMessages)
{
   if (!object)
   {
       errorMessages.push_back(L"Warning : no global properties defined!");
       return;
   }

   map<wstring, wstring>::iterator itProp = object->BeginProperties();
   map<wstring, wstring>::iterator endProp = object->EndProperties();
   for (; itProp != endProp; itProp++)
   {
       pair<wstring, wstring> currentProp = *itProp;
       if (currentProp.first == L"MasterEmail")
           masterEmail = currentProp.second;
       else if (currentProp.first == L"ReportDispatching")
           reportDispatching = currentProp.second;
       else if (currentProp.first == L"ShutdownOnFinish")
       {
           shutdown = ConfigurationTools::GetBooleanValue(currentProp.second,
                                                          errorMessages);
       }
       else if (currentProp.first != L"Type")
       {
           wstring message = L"Warning : unknown property \"";
           message += currentProp.first + L"\"";
           errorMessages.push_back(message);
       }
   }
}

bool StandaloneConfiguration::IsConfigurationConsistent(vector<wstring>& errorMessages)
{
   if (hasFatalError)
       return false;
   else if (agent == NULL)
   {
       errorMessages.push_back(ConfigurationTools::CreateError(MsgMissingAgent));
       return false;
   }
   else if (client == NULL)
   {
       errorMessages.push_back(ConfigurationTools::CreateError(MsgMissingClient));
       return false;
   }

   if (reportCreator == NULL)
   {
       reportCreator = new TextReportCreator();
       errorMessages.push_back(L"Warning : missing Report configuration. Defaulting to text");
   }

   CheckReportDispatchingErrors(errorMessages);
   return true;
}

void StandaloneConfiguration::CheckReportDispatchingErrors(
   std::vector<wstring>& errorMessages
)
{
   const bool isReportDispatchingValid = (reportDispatching == L"email" ||
                                          reportDispatching == L"file" ||
                                          reportDispatching == L"console");
   if (!isReportDispatchingValid)
   {
       wstringstream message;
       message << "Warning : unknown " << reportDispatching << " dispatching.";
       message << "Defaulting to console.";
       reportDispatching = L"console";
       errorMessages.push_back(message.str());
   }
   else if (reportDispatching == L"email" && !IsEmailDataComplete())
   {
       errorMessages.push_back(L"Error : missing data for email sending. Defaulting to console.");
       reportDispatching = L"console";
   }
   else if (reportDispatching == L"file" && !IsFileDataComplete())
   {
       errorMessages.push_back(L"Error : missing data for file storage. Defaulting to console.");
       reportDispatching = L"console";
   }
}

void StandaloneConfiguration::SaveClientToOpenedFile(wofstream& file)
{
   file << "Client" << endl;
   file << "{" << endl;
   ConfigurationTools::SaveValueToFile(file, L"Name", client->GetName());
   ConfigurationTools::SaveValueToFile(file, L"showDebugInformation", L"never");

   SaveClientPropertiesToOpenedFile(file);

   list<AbstractJob*> clientJobList;
   client->GetJobList(clientJobList);
   ConfigurationTools::SaveJobListToFile(file, clientJobList);

   file << "}" << endl;
}

void StandaloneConfiguration::SaveReportOptionsToOpenedFile(wofstream& file)
{
   file << "Report" << endl;
   file << "{" << endl;
   ConfigurationTools::SaveValueToFile(file, L"type", reportType);
   ConfigurationTools::SaveValueToFile(file, L"css", cssFile);
   file << "}" << endl;
}

void StandaloneConfiguration::SaveGlobalPropertiesToOpenedFile(wofstream& file)
{
   file << "MasterEmail = \"" << masterEmail << "\";" << endl;
   file << "ReportDispatching = \"" << reportDispatching << "\";" << endl;
   file << "ShutdownOnFinish = " << (shutdown ? "true" : "false") << ";" << endl;
}

void StandaloneConfiguration::SaveClientPropertiesToOpenedFile(wofstream& file)
{
   Client::PropertyMap::const_iterator it = client->PropertyBegin();
   Client::PropertyMap::const_iterator end = client->PropertyEnd();
   for(; it != end; ++it)
      ConfigurationTools::SaveValueToFile(file, it->first, it->second);
}

bool StandaloneConfiguration::CreateClient(ConfigurationObject *confObject,
                                           vector<wstring> &errorMessages)
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
                                          vector<wstring> &errorMessages)
{
    if (agent != NULL)
    {
        errorMessages.push_back(L"Warning : redefining Agent object");
        delete agent;
    }

    agent = new Agent(confObject, errorMessages);
}

void StandaloneConfiguration::CreateReport(ConfigurationObject *confObject,
                                           vector<wstring> &errorMessages)
{
    reportType = confObject->GetFirstProperty(L"type", L"param0");
    ChangeReportCreator();
    if (reportCreator == NULL)
    {
        wstring message = L"Warning : unsupported \"";
        message += reportType + L"\" report type. Defaulting to text";
        errorMessages.push_back(message);

        reportCreator = new TextReportCreator();
        reportType = L"text";
    }

    wstring useProfiling = confObject->GetFirstProperty(L"timed", L"param1");
    reportCreator->UseProfileColumn(useProfiling != L"false");

    cssFile = confObject->GetProperty(L"css");
    if (cssFile != L"")
    {
        HtmlReportCreator* htmlReportCreator = dynamic_cast<HtmlReportCreator*>(reportCreator);
        if (htmlReportCreator)
            htmlReportCreator->SetCssFile(cssFile);
    }
}

void StandaloneConfiguration::ChangeReportCreator()
{
   delete reportCreator;
   if (reportType == L"text")
      reportCreator = new TextReportCreator();
   else if (reportType == L"html")
      reportCreator = new HtmlReportCreator();
   else
      reportCreator = NULL;
}


bool StandaloneConfiguration::IsEmailDataComplete() const
{
   return (agent->HasValidEmailData() && masterEmail != L"");
}

bool StandaloneConfiguration::IsFileDataComplete() const
{
   return (agent->GetReportFolder() != L"");
}

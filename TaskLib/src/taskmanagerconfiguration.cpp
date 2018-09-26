#include "taskmanagerconfiguration.h"

#include "configurationparser.h"
#include "profiledjob.h"

using namespace std;

TaskManagerConfiguration::TaskManagerConfiguration()
    : client(NULL), hasFatalError(false)
{
}

TaskManagerConfiguration::~TaskManagerConfiguration()
{
   delete client;
}

bool TaskManagerConfiguration::LoadFromFile(const string &fileName, vector<string> &errorMessages)
{
    errorMessages.clear();
    Reset();

    ConfigurationParser parser;
    bool result = parser.ParseFile(fileName, errorMessages);
    if (!result)
        return false;

    return SetupData(parser, errorMessages);
}

bool TaskManagerConfiguration::LoadFromBuffer(const string &content, vector<string> &errorMessages)
{
    errorMessages.clear();
    Reset();

    ConfigurationParser parser;
    bool result = parser.ParseBuffer(content, errorMessages);
    if (!result)
        return false;

    return SetupData(parser, errorMessages);
}

bool TaskManagerConfiguration::SaveToFile(const string& filename)
{
   ofstream filestream(filename.c_str());
   if (!filestream.is_open())
      return false;

   SaveContentToOpenedFile(filestream);

   filestream.close();
   return true;
}

void TaskManagerConfiguration::GetJobList(std::list<AbstractJob*>& _jobList) const
{
   list<AbstractJob*>::const_iterator it = jobList.begin();
   for (; it != jobList.end(); ++it)
      _jobList.push_back((*it)->Clone());
}


void TaskManagerConfiguration::SetJobList(const std::vector<AbstractJob*>& jobs)
{
   ClearJobs();
   vector<AbstractJob*>::const_iterator it = jobs.begin();
   for (; it != jobs.end(); ++it)
      jobList.push_back((*it)->Clone());
}

void TaskManagerConfiguration::ClearJobs()
{
   list<AbstractJob*>::iterator it = jobList.begin();
   for (; it != jobList.end(); ++it)
      delete *it;
   jobList.clear();
}

bool TaskManagerConfiguration::HasClient() const
{
    return (client != NULL);
}

Client* TaskManagerConfiguration::GetClient() const
{
   return client;
}

void TaskManagerConfiguration::SetClient(Client* _client)
{
   client = _client;
}

string TaskManagerConfiguration::CreateWarning(const string &message) const
{
    return CreateMessage("Warning", message);
}

string TaskManagerConfiguration::CreateError(const string &message) const
{
    return CreateMessage("Error", message);
}

string TaskManagerConfiguration::CreateMessage(const string &tag, const string &message) const
{
    if (message != "")
        return tag + " : " + message;
    else
       return string("");
}

void TaskManagerConfiguration::SaveJobListToOpenedFile(ofstream& file)
{
   file << "JobList" << endl;
   file << "{" << endl;

   list<AbstractJob*>::const_iterator it = jobList.begin();
   for (; it != jobList.end(); ++it)
   {
      ConfigurationObject* confObject = jobFactory.CreateConfigurationObject(*it);
      if (confObject)
      {
         file << confObject->CreateConfigurationString(1) << endl;
         delete confObject;
      }
   }

   file << "}" << endl;
}

void TaskManagerConfiguration::Reset()
{
   jobList.clear();
   hasFatalError = false;
}

bool TaskManagerConfiguration::SetupData(const ConfigurationParser &parser,
                                         std::vector<string> &errorMessages)
{
    FillRootObjects(parser.objectList, errorMessages);
    FillGlobalProperties(parser.anonymousObject, errorMessages);
    return IsConfigurationConsistent(errorMessages);
}

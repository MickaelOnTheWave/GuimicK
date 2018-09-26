#include "clientjobsconfiguration.h"

#include <fstream>

using namespace std;

static const string JobListObjectName = "JobList";

string ClientJobsConfiguration::MsgNoJobList = "Client without job list";
string ClientJobsConfiguration::MsgUnsupportedObjects = "Unsupported objects present";

ClientJobsConfiguration::ClientJobsConfiguration(const int _debugOption)
   : debugOption(_debugOption)
{
}

ClientWorkManager* ClientJobsConfiguration::BuildWorkList(const bool withProfiling) const
{
   ClientWorkManager* workManager = new ClientWorkManager(client->Clone());
   workManager->AddJobs(jobList, withProfiling);
   return workManager;
}

bool ClientJobsConfiguration::LoadFromConfigurationObject(ConfigurationObject* confObject,
                                                          std::vector<string>& errorMessages)
{
   FillJobList(confObject, errorMessages);
   return IsConfigurationConsistent(errorMessages);
}

void ClientJobsConfiguration::FillRootObjects(const list<ConfigurationObject*> &objectList,
                                              vector<string> &errorMessages)
{
   ConfigurationObject* jobListObject = FindJobListObject(objectList);
   if (objectList.size() == 0 || jobListObject == NULL)
   {
     errorMessages.push_back(CreateWarning(MsgNoJobList));
     return;
   }

   if (objectList.size() > 1)
      errorMessages.push_back(CreateWarning(MsgUnsupportedObjects));

   FillJobList(jobListObject, errorMessages);
}

void ClientJobsConfiguration::FillGlobalProperties(ConfigurationObject *, vector<string> &)
{
}

bool ClientJobsConfiguration::IsConfigurationConsistent(std::vector<string> &)
{
   return true;
}

void ClientJobsConfiguration::SaveContentToOpenedFile(ofstream& file)
{
   SaveJobListToOpenedFile(file);
}

ConfigurationObject* ClientJobsConfiguration::FindJobListObject(
      const list<ConfigurationObject*>& objectList)
{
   list<ConfigurationObject*>::const_iterator itObjs = objectList.begin();
   for (; itObjs != objectList.end(); ++itObjs)
   {
      if ((*itObjs)->GetName() == JobListObjectName)
         return *itObjs;
   }

   return NULL;
}

void ClientJobsConfiguration::FillJobList(ConfigurationObject* jobListObj,
                                          vector<string> &errorMessages)
{
   list<ConfigurationObject*>::iterator itJobs = jobListObj->objectList.begin();
   list<ConfigurationObject*>::iterator endJobs = jobListObj->objectList.end();
   for (; itJobs != endJobs; itJobs++)
   {
       AbstractJob* parsedJob = jobFactory.CreateJob(*itJobs, errorMessages);
       if (parsedJob)
       {
          if (debugOption != DebugOutput::UNDEFINED)
            parsedJob->SetOutputDebugInformation(debugOption);
          jobList.push_back(parsedJob);
       }
   }
}

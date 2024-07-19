#include "clientjobsconfiguration.h"

#include <fstream>
#include "configurationtools.h"
#include "jobfactory.h"

using namespace std;

static const wstring JobListObjectName = L"JobList";

wstring ClientJobsConfiguration::MsgNoJobList = L"Client without job list";
wstring ClientJobsConfiguration::MsgUnsupportedObjects = L"Unsupported objects present";

ClientJobsConfiguration::ClientJobsConfiguration(const int _debugOption)
   : AbstractTypeConfiguration(), debugOption(_debugOption)
{
}

ClientJobsConfiguration::ClientJobsConfiguration(const ClientJobsConfiguration& other)
   : AbstractTypeConfiguration(other),
     debugOption(other.debugOption)
{
   std::list<AbstractJob*>::const_iterator it = other.jobList.begin();
   for (; it != other.jobList.end(); ++it)
      jobList.push_back((*it)->Clone());
}

ConfigurationType ClientJobsConfiguration::GetType() const
{
   return ClientConfigurationType;
}

AbstractTypeConfiguration* ClientJobsConfiguration::Copy() const
{
   return new ClientJobsConfiguration(*this);
}

bool ClientJobsConfiguration::LoadFromBuffer(const wstring& buffer,
                                             vector<wstring>& errorMessages)
{
   ConfigurationParser parser;
   bool ok = parser.ParseBuffer(buffer, errorMessages);
   if (ok)
       return Load(parser, errorMessages);
   else
      return false;
}

void ClientJobsConfiguration::SaveToOpenedFile(wofstream& file)
{
   ConfigurationTools::SaveJobListToFile(file, jobList);
}

void ClientJobsConfiguration::GetJobList(std::list<AbstractJob*>& _jobList)
{
   copy(jobList.begin(), jobList.end(), back_inserter(_jobList));
}

void ClientJobsConfiguration::SetJobList(const std::vector<AbstractJob*>& _jobList)
{
   jobList.clear();
   copy(_jobList.begin(), _jobList.end(), back_inserter(jobList));
}

void ClientJobsConfiguration::ClearJobList()
{
   list<AbstractJob*>::iterator it=jobList.begin();
   for (; it!=jobList.end(); ++it)
      delete *it;
   jobList.clear();
}

bool ClientJobsConfiguration::LoadFromConfigurationObject(ConfigurationObject* confObject,
                                                          std::vector<wstring>& errorMessages)
{
   FillJobList(confObject, errorMessages);
   return IsConfigurationConsistent(errorMessages);
}

void ClientJobsConfiguration::FillRootObjects(const list<ConfigurationObject*> &objectList,
                                              vector<wstring> &errorMessages)
{
   ConfigurationObject* jobListObject = FindJobListObject(objectList);
   if (objectList.size() == 0 || jobListObject == NULL)
   {
     errorMessages.push_back(ConfigurationTools::CreateWarning(MsgNoJobList));
     return;
   }

   if (objectList.size() > 1)
      errorMessages.push_back(ConfigurationTools::CreateWarning(MsgUnsupportedObjects));

   FillJobList(jobListObject, errorMessages);
}

void ClientJobsConfiguration::FillGlobalProperties(ConfigurationObject *, vector<wstring> &)
{
}

bool ClientJobsConfiguration::IsConfigurationConsistent(std::vector<wstring> &)
{
   return true;
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
                                          vector<wstring> &errorMessages)
{
   JobFactory jobFactory;
   list<ConfigurationObject*>::iterator itJobs = jobListObj->BeginObjects();
   list<ConfigurationObject*>::iterator endJobs = jobListObj->EndObjects();
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

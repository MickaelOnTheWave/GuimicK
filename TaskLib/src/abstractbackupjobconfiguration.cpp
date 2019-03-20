#include "abstractbackupjobconfiguration.h"

using namespace std;

const string AbstractBackupJobConfiguration::TargetProperty = "target";
const string AbstractBackupJobConfiguration::JoinReportsProperty = "joinReports";

static const wstring invalidSourceError = "Error : source is invalid";
static const wstring invalidDestinationError = "Error : destination is invalid";

AbstractBackupJobConfiguration::AbstractBackupJobConfiguration(const std::string &tag)
    : AbstractJobDefaultConfiguration(tag)
{
}

ConfigurationObject* AbstractBackupJobConfiguration::CreateConfigurationObject(AbstractJob* job)
{
   AbstractBackupJob* castJob = static_cast<AbstractBackupJob*>(job);
   ConfigurationObject* confObject = AbstractJobDefaultConfiguration::CreateConfigurationObject(job);

   const string targetValue = (castJob->IsTargetLocal() ? "local" : "remote");
   confObject->SetProperty(TargetProperty, targetValue);
   confObject->SetProperty(JoinReportsProperty, (castJob->GetJoinReports() ? "true" : "false"));

   vector<pair<string,string> > backupPoints;
   castJob->GetFolderList(backupPoints);
   vector<pair<string,string> >::const_iterator it = backupPoints.begin();
   for (; it != backupPoints.end(); ++it)
   {
      ConfigurationObject* subObject = new ConfigurationObject(GetBackupItemName());
      subObject->SetProperty("source", it->first);
      subObject->SetProperty("dest", it->second);
      confObject->AddObject(subObject);
   }
   return confObject;
}

void AbstractBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(TargetProperty);
    properties.push_back(JoinReportsProperty);
}

void AbstractBackupJobConfiguration::FillKnownSubObjects(std::vector<string> &objects)
{
    objects.push_back(GetBackupItemName());
}

void AbstractBackupJobConfiguration::ConfigureJob(AbstractJob *job,
                                                  ConfigurationObject *confObject,
                                                  std::vector<string> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    AbstractBackupJob* castJob = static_cast<AbstractBackupJob*>(job);

    ConfigureItemList(castJob, confObject, errorMessages);
    ConfigureTarget(castJob, confObject);
    castJob->SetJoinReports(confObject->GetProperty(JoinReportsProperty) == "true");
}

void AbstractBackupJobConfiguration::ConfigureItemList(AbstractBackupJob *job,
                                                       ConfigurationObject *confObject,
                                                       vector<string> &errorMessages)
{
    list<ConfigurationObject*>::const_iterator it = confObject->objectList.begin();
    for (; it != confObject->objectList.end(); ++it)
    {
        ConfigurationObject* currentObj = *it;

        if (currentObj->name == GetBackupItemName())
        {
            const string source(currentObj->GetProperty("source"));
            const string dest(currentObj->GetProperty("dest"));

            const bool isSourceValid = (source != "");
            const bool isDestinationValid = (dest != "");

            if (isSourceValid && isDestinationValid)
                job->AddFolder(source, dest);
            else if (isSourceValid == false)
                errorMessages.push_back(invalidSourceError);
            else // isDestinationValid == false
                errorMessages.push_back(invalidDestinationError);
        }
    }
}

void AbstractBackupJobConfiguration::ConfigureTarget(AbstractBackupJob *job,
                                                     ConfigurationObject *confObject)
{
    string target(confObject->GetProperty(TargetProperty));
    if (target == "local")
        job->SetTargetLocal();
    else
        job->SetTargetRemote();
}

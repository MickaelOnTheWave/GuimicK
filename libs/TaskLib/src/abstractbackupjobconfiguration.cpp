#include "abstractbackupjobconfiguration.h"

using namespace std;

const wstring AbstractBackupJobConfiguration::TargetProperty = L"target";
const wstring AbstractBackupJobConfiguration::JoinReportsProperty = L"joinReports";

static const wstring invalidSourceError = L"Error : source is invalid";
static const wstring invalidDestinationError = L"Error : destination is invalid";

AbstractBackupJobConfiguration::AbstractBackupJobConfiguration(const std::wstring &tag)
    : AbstractJobDefaultConfiguration(tag)
{
}

ConfigurationObject* AbstractBackupJobConfiguration::CreateConfigurationObject(AbstractJob* job)
{
   AbstractBackupJob* castJob = static_cast<AbstractBackupJob*>(job);
   ConfigurationObject* confObject = AbstractJobDefaultConfiguration::CreateConfigurationObject(job);

   const wstring targetValue = (castJob->IsTargetLocal() ? L"local" : L"remote");
   confObject->SetProperty(TargetProperty, targetValue);
   confObject->SetProperty(JoinReportsProperty, (castJob->GetJoinReports() ? L"true" : L"false"));

   vector<pair<wstring,wstring> > backupPoints;
   castJob->GetFolderList(backupPoints);
   vector<pair<wstring,wstring> >::const_iterator it = backupPoints.begin();
   for (; it != backupPoints.end(); ++it)
   {
      ConfigurationObject* subObject = new ConfigurationObject(GetBackupItemName());
      subObject->SetProperty(L"source", it->first);
      subObject->SetProperty(L"dest", it->second);
      confObject->AddObject(subObject);
   }
   return confObject;
}

void AbstractBackupJobConfiguration::FillKnownProperties(std::vector<std::wstring> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(TargetProperty);
    properties.push_back(JoinReportsProperty);
}

void AbstractBackupJobConfiguration::FillKnownSubObjects(std::vector<wstring> &objects)
{
    objects.push_back(GetBackupItemName());
}

void AbstractBackupJobConfiguration::ConfigureJob(AbstractJob *job,
                                                  ConfigurationObject *confObject,
                                                  std::vector<wstring> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    AbstractBackupJob* castJob = static_cast<AbstractBackupJob*>(job);

    ConfigureItemList(castJob, confObject, errorMessages);
    ConfigureTarget(castJob, confObject);
    castJob->SetJoinReports(confObject->GetProperty(JoinReportsProperty) == L"true");
}

void AbstractBackupJobConfiguration::ConfigureItemList(AbstractBackupJob *job,
                                                       ConfigurationObject *confObject,
                                                       vector<wstring> &errorMessages)
{
    list<ConfigurationObject*>::const_iterator it = confObject->BeginObjects();
    for (; it != confObject->EndObjects(); ++it)
    {
        ConfigurationObject* currentObj = *it;

        if (currentObj->GetName() == GetBackupItemName())
        {
            const wstring source(currentObj->GetProperty(L"source"));
            const wstring dest(currentObj->GetProperty(L"dest"));

            const bool isSourceValid = (source != L"");
            const bool isDestinationValid = (dest != L"");

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
    wstring target(confObject->GetProperty(TargetProperty));
    if (target == L"local")
        job->SetTargetLocal();
    else
        job->SetTargetRemote();
}

#include "rsnapshotbackupjobconfiguration.h"

#include <stdlib.h>
#include "rsnapshotrawbackupjob.h"
#include "rsnapshotsmartbackupjob.h"
#include "stringtools.h"

using namespace std;

const wstring RsnapshotBackupJobConfiguration::FullConfigurationProperty = L"fullConfigurationFile";
const wstring RsnapshotBackupJobConfiguration::WaitProperty = L"waitAfterRun";
const wstring RsnapshotBackupJobConfiguration::TemplateConfigurationProperty = L"templateConfigurationFile";
const wstring RsnapshotBackupJobConfiguration::RepositoryProperty = L"repository";
const wstring RsnapshotBackupJobConfiguration::MaxBackupCountProperty = L"maxBackupCount";
const wstring RsnapshotBackupJobConfiguration::TemporaryFile = L"temporaryFile";

RsnapshotBackupJobConfiguration::RsnapshotBackupJobConfiguration()
    : AbstractBackupJobConfiguration(L"RsnapshotBackup"),
      fullConfigurationFile(L"")
{
}

bool RsnapshotBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   const bool isSmartJob = (dynamic_cast<RsnapshotSmartBackupJob*>(job) != NULL);
   const bool isRawJob = (dynamic_cast<RsnapshotRawBackupJob*>(job) != NULL);
   return isSmartJob || isRawJob;
}

ConfigurationObject* RsnapshotBackupJobConfiguration::CreateConfigurationObject(AbstractJob* job)
{
   ConfigurationObject* confObject = AbstractBackupJobConfiguration::CreateConfigurationObject(job);
   RsnapshotRawBackupJob* rawJob = dynamic_cast<RsnapshotRawBackupJob*>(job);
   if (rawJob != NULL)
      CreateRawConfiguration(confObject, rawJob);
   else
   {
      RsnapshotSmartBackupJob* smartJob = dynamic_cast<RsnapshotSmartBackupJob*>(job);
      if (smartJob != NULL)
         CreateSmartConfiguration(confObject, smartJob);
   }

   return confObject;
}

void RsnapshotBackupJobConfiguration::AnalyzeConfiguration(ConfigurationObject *confObject)
{
    fullConfigurationFile = confObject->GetFirstProperty(FullConfigurationProperty, L"param1");
}

AbstractJob *RsnapshotBackupJobConfiguration::CreateJob()
{
    if (fullConfigurationFile != L"")
        return new RsnapshotRawBackupJob();
    else
        return new RsnapshotSmartBackupJob();
}

void RsnapshotBackupJobConfiguration::ConfigureJob(AbstractJob *job, ConfigurationObject *confObject, std::vector<wstring> &errorMessages)
{
    if (fullConfigurationFile != L"")
        ConfigureRawJob(static_cast<RsnapshotRawBackupJob*>(job), confObject, errorMessages);
    else
        ConfigureSmartJob(static_cast<RsnapshotSmartBackupJob*>(job), confObject, errorMessages);
}

void RsnapshotBackupJobConfiguration::FillKnownProperties(std::vector<std::wstring> &properties)
{
    AbstractBackupJobConfiguration::FillKnownProperties(properties);
    properties.push_back(RepositoryProperty);
    properties.push_back(FullConfigurationProperty);
    properties.push_back(WaitProperty);
    properties.push_back(TemplateConfigurationProperty);
    properties.push_back(MaxBackupCountProperty);
    properties.push_back(TemporaryFile);
}

wstring RsnapshotBackupJobConfiguration::GetBackupItemName() const
{
    return wstring(L"Folder");
}

void RsnapshotBackupJobConfiguration::ConfigureSmartJob(RsnapshotSmartBackupJob *job,
                                                        ConfigurationObject *confObject,
                                                        std::vector<wstring> &errorMessages)
{
    AbstractBackupJobConfiguration::ConfigureJob(job, confObject, errorMessages);

    const wstring templateFile = confObject->GetProperty(TemplateConfigurationProperty);
    job->SetTemplateConfigurationFile(templateFile);

    job->SetRepository(GetRepositoryValue(confObject));
    job->SetWaitBeforeRun(GetWaitBeforeRunValue(confObject));
    SetMaxBackupCount(job, confObject);
    job->SetTemporaryFile(confObject->GetProperty(TemporaryFile));
}

void RsnapshotBackupJobConfiguration::ConfigureRawJob(RsnapshotRawBackupJob *job,
                                                      ConfigurationObject *confObject,
                                                      std::vector<wstring> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    job->SetConfigurationFile(fullConfigurationFile);
    job->SetRepository(GetRepositoryValue(confObject));
    job->SetWaitBeforeRun(GetWaitBeforeRunValue(confObject));
}

wstring RsnapshotBackupJobConfiguration::GetRepositoryValue(ConfigurationObject *confObject) const
{
    return confObject->GetFirstProperty(RepositoryProperty, L"param0");
}

bool RsnapshotBackupJobConfiguration::GetWaitBeforeRunValue(ConfigurationObject *confObject) const
{
    return (confObject->GetProperty(WaitProperty) == L"true");
}

void RsnapshotBackupJobConfiguration::SetMaxBackupCount(RsnapshotSmartBackupJob *job,
                                                        ConfigurationObject *confObject) const
{
    const wstring maxBackupCountProperty = confObject->GetProperty(MaxBackupCountProperty);
    if (maxBackupCountProperty != L"")
    {
        int intMaxBackupCount = StringTools::ToInt(maxBackupCountProperty);
        if (intMaxBackupCount != 0)
        {
            if (intMaxBackupCount < 1)
                intMaxBackupCount = 1;
            job->SetMaxBackupCount(intMaxBackupCount);
        }
    }
}

void RsnapshotBackupJobConfiguration::CreateRawConfiguration(
      ConfigurationObject* conf, RsnapshotRawBackupJob* job)
{
   conf->SetProperty(FullConfigurationProperty, fullConfigurationFile);
   conf->SetProperty(RepositoryProperty, job->GetRepository());
}

void RsnapshotBackupJobConfiguration::CreateSmartConfiguration(
      ConfigurationObject* conf, RsnapshotSmartBackupJob* job)
{
   conf->SetProperty(RepositoryProperty, job->GetRepository());

   wstringstream maxBackupCountStr;
   maxBackupCountStr << job->GetMaxBackupCount();
   conf->SetProperty(MaxBackupCountProperty, maxBackupCountStr.str());
   conf->SetProperty(TemporaryFile, job->GetTemporaryFile());
}

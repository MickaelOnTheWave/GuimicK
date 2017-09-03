#include "rsnapshotbackupjobconfiguration.h"

#include <stdlib.h>
#include "rsnapshotrawbackupjob.h"
#include "rsnapshotsmartbackupjob.h"

using namespace std;

const string RsnapshotBackupJobConfiguration::FullConfigurationProperty = "fullConfigurationFile";
const string RsnapshotBackupJobConfiguration::WaitProperty = "waitAfterRun";
const string RsnapshotBackupJobConfiguration::TemplateConfigurationProperty = "templateConfigurationFile";
const string RsnapshotBackupJobConfiguration::RepositoryProperty = "repository";
const string RsnapshotBackupJobConfiguration::MaxBackupCountProperty = "maxBackupCount";

RsnapshotBackupJobConfiguration::RsnapshotBackupJobConfiguration()
    : AbstractBackupJobConfiguration("RsnapshotBackup"),
      fullConfigurationFile("")
{
}

void RsnapshotBackupJobConfiguration::AnalyzeConfiguration(ConfigurationObject *confObject)
{
    fullConfigurationFile = confObject->GetFirstProperty(FullConfigurationProperty, "param1");
}

AbstractJob *RsnapshotBackupJobConfiguration::CreateJob()
{
    if (fullConfigurationFile != "")
        return new RsnapshotRawBackupJob();
    else
        return new RsnapshotSmartBackupJob();
}

void RsnapshotBackupJobConfiguration::ConfigureJob(AbstractJob *job, ConfigurationObject *confObject, std::vector<string> &errorMessages)
{
    if (fullConfigurationFile != "")
        ConfigureRawJob(static_cast<RsnapshotRawBackupJob*>(job), confObject, errorMessages);
    else
        ConfigureSmartJob(static_cast<RsnapshotSmartBackupJob*>(job), confObject, errorMessages);
}

void RsnapshotBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    AbstractBackupJobConfiguration::FillKnownProperties(properties);
    properties.push_back(RepositoryProperty);
    properties.push_back(FullConfigurationProperty);
    properties.push_back(WaitProperty);
    properties.push_back(TemplateConfigurationProperty);
    properties.push_back(MaxBackupCountProperty);
}

string RsnapshotBackupJobConfiguration::GetBackupItemName() const
{
    return string("Folder");
}

void RsnapshotBackupJobConfiguration::ConfigureSmartJob(RsnapshotSmartBackupJob *job,
                                                        ConfigurationObject *confObject,
                                                        std::vector<string> &errorMessages)
{
    AbstractBackupJobConfiguration::ConfigureJob(job, confObject, errorMessages);

    const string templateFile = confObject->GetProperty(TemplateConfigurationProperty);
    job->SetTemplateConfigurationFile(templateFile);

    job->SetRepository(GetRepositoryValue(confObject));
    job->SetWaitBeforeRun(GetWaitBeforeRunValue(confObject));
    SetMaxBackupCount(job, confObject);
}

void RsnapshotBackupJobConfiguration::ConfigureRawJob(RsnapshotRawBackupJob *job,
                                                      ConfigurationObject *confObject,
                                                      std::vector<string> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    job->SetConfigurationFile(fullConfigurationFile);
    job->SetRepository(GetRepositoryValue(confObject));
    job->SetWaitBeforeRun(GetWaitBeforeRunValue(confObject));
}

string RsnapshotBackupJobConfiguration::GetRepositoryValue(ConfigurationObject *confObject) const
{
    return confObject->GetFirstProperty(RepositoryProperty, "param0");
}

bool RsnapshotBackupJobConfiguration::GetWaitBeforeRunValue(ConfigurationObject *confObject) const
{
    return (confObject->GetProperty(WaitProperty) == "true");
}

void RsnapshotBackupJobConfiguration::SetMaxBackupCount(RsnapshotSmartBackupJob *job,
                                                        ConfigurationObject *confObject) const
{
    const string maxBackupCountProperty = confObject->GetProperty(MaxBackupCountProperty);
    if (maxBackupCountProperty != "")
    {
        int intMaxBackupCount = atoi(maxBackupCountProperty.c_str());
        if (intMaxBackupCount != 0)
        {
            if (intMaxBackupCount < 1)
                intMaxBackupCount = 1;
            job->SetMaxBackupCount(intMaxBackupCount);
        }
    }
}

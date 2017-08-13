#include "rsnapshotbackupjobconfiguration.h"

#include "rsnapshotrawbackupjob.h"
#include "rsnapshotsmartbackupjob.h"

using namespace std;

static const string fullConfigurationProperty = "fullConfigurationFile";
static const string waitProperty = "waitAfterRun";
static const string templateConfigurationProperty = "templateConfigurationFile";
static const string repositoryProperty = "repository";

RsnapshotBackupJobConfiguration::RsnapshotBackupJobConfiguration()
    : AbstractBackupJobConfiguration("RsnapshotBackup"),
      fullConfigurationFile("")
{
}

void RsnapshotBackupJobConfiguration::AnalyzeConfiguration(ConfigurationObject *confObject)
{
    fullConfigurationFile = confObject->GetFirstProperty(fullConfigurationProperty, "param1");
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
    properties.push_back(repositoryProperty);
    properties.push_back(fullConfigurationProperty);
    properties.push_back(waitProperty);
    properties.push_back(templateConfigurationProperty);
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

    const string templateFile = confObject->GetProperty(templateConfigurationProperty);
    job->SetTemplateConfigurationFile(templateFile);

    job->SetRepository(GetRepositoryValue(confObject));
}

void RsnapshotBackupJobConfiguration::ConfigureRawJob(RsnapshotRawBackupJob *job,
                                                      ConfigurationObject *confObject,
                                                      std::vector<string> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    job->SetConfigurationFile(fullConfigurationFile);
    job->SetRepository(GetRepositoryValue(confObject));
    job->SetWaitAfterRun(GetWaitAfterRunValue(confObject));
}

string RsnapshotBackupJobConfiguration::GetRepositoryValue(ConfigurationObject *confObject) const
{
    return confObject->GetFirstProperty(repositoryProperty, "param0");
}

bool RsnapshotBackupJobConfiguration::GetWaitAfterRunValue(ConfigurationObject *confObject) const
{
    return (confObject->GetProperty(waitProperty) == "true");
}

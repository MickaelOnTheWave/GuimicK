#include "gitbackupjobconfiguration.h"
#include "gitbackupjob.h"

using namespace std;

const string GitBackupJobConfiguration::WriteLogProperty = "writeLogsToFiles";

GitBackupJobConfiguration::GitBackupJobConfiguration()
    : AbstractBackupJobConfiguration("GitBackup")
{
}

AbstractJob *GitBackupJobConfiguration::CreateJob()
{
    return new GitBackupJob();
}

void GitBackupJobConfiguration::ConfigureJob(AbstractJob *job,
                                             ConfigurationObject *confObject,
                                             std::vector<string> &errorMessages)
{
    AbstractBackupJobConfiguration::ConfigureJob(job, confObject, errorMessages);

    GitBackupJob* castJob = static_cast<GitBackupJob*>(job);
    const string writeLogsToFiles(confObject->GetProperty(WriteLogProperty));
    if (writeLogsToFiles == "true")
        castJob->SetWriteLogsToFiles(true);

}

void GitBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    AbstractBackupJobConfiguration::FillKnownProperties(properties);
    properties.push_back(WriteLogProperty);
}

string GitBackupJobConfiguration::GetBackupItemName() const
{
    return string("Repository");
}

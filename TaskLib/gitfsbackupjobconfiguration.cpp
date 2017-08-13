#include "gitfsbackupjobconfiguration.h"
#include "gitfsbackupjob.h"

using namespace std;

GitFsBackupJobConfiguration::GitFsBackupJobConfiguration()
    : AbstractBackupJobConfiguration("GitFsBackup")
{
}

AbstractJob *GitFsBackupJobConfiguration::CreateJob()
{
    return new GitFsBackupJob();
}

void GitFsBackupJobConfiguration::ConfigureJob(AbstractJob *job,
                                               ConfigurationObject *confObject,
                                               vector<string> &errorMessages)
{
    AbstractBackupJobConfiguration::ConfigureJob(job, confObject, errorMessages);
}

void GitFsBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    AbstractBackupJobConfiguration::FillKnownProperties(properties);

}

string GitFsBackupJobConfiguration::GetBackupItemName() const
{
    return string("Folder");
}

#include "gitfsbackupjobconfiguration.h"
#include "gitfsbackupjob.h"

using namespace std;

GitFsBackupJobConfiguration::GitFsBackupJobConfiguration()
    : AbstractBackupJobConfiguration("GitFsBackup")
{
}

bool GitFsBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<GitFsBackupJob*>(job) != NULL);
}

AbstractJob *GitFsBackupJobConfiguration::CreateJob()
{
    return new GitFsBackupJob();
}

string GitFsBackupJobConfiguration::GetBackupItemName() const
{
    return string("Folder");
}

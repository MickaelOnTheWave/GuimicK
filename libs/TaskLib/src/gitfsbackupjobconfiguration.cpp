#include "gitfsbackupjobconfiguration.h"
#include "gitfsbackupjob.h"

using namespace std;

GitFsBackupJobConfiguration::GitFsBackupJobConfiguration()
    : AbstractBackupJobConfiguration(L"GitFsBackup")
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

wstring GitFsBackupJobConfiguration::GetBackupItemName() const
{
    return L"Folder";
}

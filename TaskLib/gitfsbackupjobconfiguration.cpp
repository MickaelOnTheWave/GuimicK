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

string GitFsBackupJobConfiguration::GetBackupItemName() const
{
    return string("Folder");
}

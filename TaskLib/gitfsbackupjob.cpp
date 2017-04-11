#include "gitfsbackupjob.h"

using namespace std;

GitFsBackupJob::GitFsBackupJob()
{
}

GitFsBackupJob::GitFsBackupJob(const GitFsBackupJob &)
{
}

std::string GitFsBackupJob::GetName()
{
    return string("Git Filesystem Backup");
}

AbstractJob *GitFsBackupJob::Clone()
{
    return new GitFsBackupJob(*this);
}

bool GitFsBackupJob::InitializeFromClient(Client *)
{
    return IsInitialized();
}

bool GitFsBackupJob::IsInitialized()
{
    return false;
}

JobStatus *GitFsBackupJob::Run()
{
    return new JobStatus();
}

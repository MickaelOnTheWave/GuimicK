#include "copyfsbackupjob.h"

using namespace std;

CopyFsBackupJob::CopyFsBackupJob()
{
}

CopyFsBackupJob::CopyFsBackupJob(const CopyFsBackupJob &other)
    : AbstractBackupJob(other)
{
}

std::string CopyFsBackupJob::GetName()
{
    return string("CopyFsBackup");
}

AbstractJob *CopyFsBackupJob::Clone()
{
    return new CopyFsBackupJob(*this);
}

JobStatus *CopyFsBackupJob::Run()
{
    return new JobStatus(JobStatus::ERROR, "Not implemented yet!");
}

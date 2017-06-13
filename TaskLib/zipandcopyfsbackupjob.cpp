#include "zipandcopyfsbackupjob.h"

using namespace std;

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob() : AbstractBackupJob("ZipCopyFsBackup")
{
}

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob(const ZipAndCopyFsBackupJob& other)
    : AbstractBackupJob(other)
{
}

std::string ZipAndCopyFsBackupJob::GetName()
{
    return string("ZipAndCopy Backup");
}

AbstractJob *ZipAndCopyFsBackupJob::Clone()
{
    return new ZipAndCopyFsBackupJob(*this);
}

bool ZipAndCopyFsBackupJob::Restore(const string &backupFile, const string &destination)
{
    // To implement
    return false;
}

void ZipAndCopyFsBackupJob::RunRepositoryBackup(const std::string &source,
                                                const std::string &destination,
                                                AbstractBackupJob::ResultCollection &results)
{
    // to implement
}

JobStatus *ZipAndCopyFsBackupJob::CreateGlobalStatus(
        const AbstractBackupJob::ResultCollection &results)
{
    // to implement
}

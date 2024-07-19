#include "rawcopyfsbackupjob.h"

#include "tools.h"

using namespace std;

static const wstring defaultName = L"Copy Backup";

AbstractRawCopyFsBackupJob::AbstractRawCopyFsBackupJob() : AbstractCopyFsBackupJob(defaultName)
{
}

AbstractRawCopyFsBackupJob::AbstractRawCopyFsBackupJob(const AbstractCopyFsBackupJob &other)
    : AbstractCopyFsBackupJob(other)
{
}

wstring AbstractRawCopyFsBackupJob::GetTypeName() const
{
   return defaultName;
}

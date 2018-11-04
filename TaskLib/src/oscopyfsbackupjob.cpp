#include "rawcopyfsbackupjob.h"

#include "tools.h"

using namespace std;

static const string defaultName = "Copy Backup";

AbstractRawCopyFsBackupJob::AbstractRawCopyFsBackupJob() : AbstractCopyFsBackupJob(defaultName)
{
}

AbstractRawCopyFsBackupJob::AbstractRawCopyFsBackupJob(const AbstractCopyFsBackupJob &other)
    : AbstractCopyFsBackupJob(other)
{
}

string AbstractRawCopyFsBackupJob::GetTypeName() const
{
   return defaultName;
}

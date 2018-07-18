#include "copyjobchooser.h"

#include "rawcopyfsbackupjob.h"
#include "rsynccopyfsbackupjob.h"

AbstractCopyFsBackupJob *CopyJobChooser::GetBestAvailable()
{
    if (RsyncCopyFsBackupJob::IsAvailable())
        return new RsyncCopyFsBackupJob();
    else
        return new RawCopyFsBackupJob();
}

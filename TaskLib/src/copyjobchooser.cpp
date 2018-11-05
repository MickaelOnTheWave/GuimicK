#include "copyjobchooser.h"

#include "oscopyfsbackupjob.h"

#include "rsynccopyfsbackupjob.h"

AbstractCopyFsBackupJob *CopyJobChooser::GetBestAvailable()
{
    if (RsyncCopyFsBackupJob::IsAvailable())
       return new RsyncCopyFsBackupJob();
    else
       return new OsCopyFsBackupJob();
}

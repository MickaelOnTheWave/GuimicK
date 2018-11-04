#include "copyjobchooser.h"

#ifdef _WIN32
   #include "WindowsCopyFsBackupJob.h"
#else
   #include "linuxcopyfsbackupjob.h"
#endif

#include "rsynccopyfsbackupjob.h"

AbstractCopyFsBackupJob *CopyJobChooser::GetBestAvailable()
{
    if (RsyncCopyFsBackupJob::IsAvailable())
       return new RsyncCopyFsBackupJob();
    else
#ifdef _WIN32
       return new WindowsCopyFsBackupJob();
#else
       return new LinuxCopyFsBackupJob();
#endif
}

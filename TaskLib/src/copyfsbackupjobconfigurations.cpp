#include "copyfsbackupjobconfigurations.h"

#ifdef _WIN32
   #include "WindowsCopyFsBackupJob.h"
#else
   #include "linuxcopyfsbackupjob.h"
#endif
#include "rsynccopyfsbackupjob.h"
#include "zipandcopyfsbackupjob.h"

//-------------------------------------------------------------------------//

RawCopyFsBackupJobConfiguration::RawCopyFsBackupJobConfiguration()
    : AbstractFsBackupJobConfiguration("CopyBackup")
{
}

bool RawCopyFsBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<AbstractRawCopyFsBackupJob*>(job) != NULL);
}

AbstractJob *RawCopyFsBackupJobConfiguration::CreateJob()
{
#ifdef _WIN32
   return new WindowsCopyFsBackupJob();
#else
   return new LinuxCopyFsBackupJob();
#endif
}

//-------------------------------------------------------------------------//

RsyncCopyFsBackupJobConfiguration::RsyncCopyFsBackupJobConfiguration()
    : AbstractFsBackupJobConfiguration("RsyncBackup")
{
}

bool RsyncCopyFsBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<RsyncCopyFsBackupJob*>(job) != NULL);
}

AbstractJob *RsyncCopyFsBackupJobConfiguration::CreateJob()
{
    return new RsyncCopyFsBackupJob();
}

//-------------------------------------------------------------------------//

ZipAndCopyFsBackupJobConfiguration::ZipAndCopyFsBackupJobConfiguration()
    : AbstractFsBackupJobConfiguration("ArchiveBackup")
{
}

bool ZipAndCopyFsBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<ZipAndCopyFsBackupJob*>(job) != NULL);
}

AbstractJob *ZipAndCopyFsBackupJobConfiguration::CreateJob()
{
    return new ZipAndCopyFsBackupJob();
}

//-------------------------------------------------------------------------//

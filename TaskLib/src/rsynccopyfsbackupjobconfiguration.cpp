#include "rsynccopyfsbackupjobconfiguration.h"

#include "rsynccopyfsbackupjob.h"

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

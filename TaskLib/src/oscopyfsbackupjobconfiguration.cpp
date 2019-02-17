#include "oscopyfsbackupjobconfiguration.h"
#include "oscopyfsbackupjob.h"

OsCopyFsBackupJobConfiguration::OsCopyFsBackupJobConfiguration()
    : AbstractFsBackupJobConfiguration("CopyBackup")
{
}

bool OsCopyFsBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<AbstractRawCopyFsBackupJob*>(job) != NULL);
}

AbstractJob *OsCopyFsBackupJobConfiguration::CreateJob()
{
   return new OsCopyFsBackupJob();
}

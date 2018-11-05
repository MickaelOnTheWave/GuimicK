#include "copyfsbackupjobconfigurations.h"

#include "oscopyfsbackupjob.h"
#include "rsynccopyfsbackupjob.h"
#include "zipandcopyfsbackupjob.h"

//-------------------------------------------------------------------------//

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

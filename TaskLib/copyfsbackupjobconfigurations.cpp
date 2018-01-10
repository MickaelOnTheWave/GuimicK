#include "copyfsbackupjobconfigurations.h"

#include "rawcopyfsbackupjob.h"
#include "rsynccopyfsbackupjob.h"
#include "zipandcopyfsbackupjob.h"

//-------------------------------------------------------------------------//

RawCopyFsBackupJobConfiguration::RawCopyFsBackupJobConfiguration()
    : AbstractFsBackupJobConfiguration("CopyBackup")
{
}

bool RawCopyFsBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<RawCopyFsBackupJob*>(job) != NULL);
}

AbstractJob *RawCopyFsBackupJobConfiguration::CreateJob()
{
    return new RawCopyFsBackupJob();
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

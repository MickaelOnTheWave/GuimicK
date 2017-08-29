#include "copyfsbackupjobconfigurations.h"

#include "rawcopyfsbackupjob.h"
#include "rsynccopyfsbackupjob.h"
#include "zipandcopyfsbackupjob.h"

//-------------------------------------------------------------------------//

RawCopyFsBackupJobConfiguration::RawCopyFsBackupJobConfiguration()
    : AbstractFsBackupJobConfiguration("CopyBackup")
{
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

AbstractJob *RsyncCopyFsBackupJobConfiguration::CreateJob()
{
    return new RsyncCopyFsBackupJob();
}

//-------------------------------------------------------------------------//

ZipAndCopyFsBackupJobConfiguration::ZipAndCopyFsBackupJobConfiguration()
    : AbstractFsBackupJobConfiguration("ArchiveBackup")
{
}

AbstractJob *ZipAndCopyFsBackupJobConfiguration::CreateJob()
{
    return new ZipAndCopyFsBackupJob();
}

//-------------------------------------------------------------------------//

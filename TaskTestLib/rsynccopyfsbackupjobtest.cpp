#include "rsynccopyfsbackupjobtest.h"

#include "rsynccopyfsbackupjob.h"

const std::string suitePrefix = "RsyncCopyFsBackup/";

RsyncCopyFsBackupJobTest::RsyncCopyFsBackupJobTest(const std::string &dataPrefix)
    : AbstractCopyFsBackupJobTest(dataPrefix + suitePrefix)
{
}

RsyncCopyFsBackupJobTest::~RsyncCopyFsBackupJobTest()
{
}

AbstractCopyFsBackupJob *RsyncCopyFsBackupJobTest::CreateCopyJob()
{
    return new RsyncCopyFsBackupJob();
}

#include "rsynccopyfsbackupjobtest.h"

#include "rsynccopyfsbackupjob.h"

const std::string suitePrefix = "RsyncCopyFsBackup/";

RsyncCopyFsBackupJobTest::RsyncCopyFsBackupJobTest(const std::string &dataPrefix,
                                                   const std::string &errorPrefix)
    : AbstractCopyFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
{
}

RsyncCopyFsBackupJobTest::~RsyncCopyFsBackupJobTest()
{
}

AbstractCopyFsBackupJob *RsyncCopyFsBackupJobTest::CreateCopyJob()
{
    return new RsyncCopyFsBackupJob();
}

AbstractCopyFsBackupJob *RsyncCopyFsBackupJobTest::CreateNewJob()
{
    return new RsyncCopyFsBackupJob();
}

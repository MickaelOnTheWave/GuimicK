#include "rawcopyfsbackupjobtest.h"

#include "rawcopyfsbackupjob.h"

const std::string suitePrefix = "RawCopyFsBackup/";

RawCopyFsBackupJobTest::RawCopyFsBackupJobTest(const std::string &dataPrefix)
    : AbstractCopyFsBackupJobTest(dataPrefix + suitePrefix)
{
}

RawCopyFsBackupJobTest::~RawCopyFsBackupJobTest()
{
}

AbstractCopyFsBackupJob *RawCopyFsBackupJobTest::CreateCopyJob()
{
    return new RawCopyFsBackupJob();
}

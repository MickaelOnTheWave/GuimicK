#include "rawcopyfsbackupjobtest.h"

#include "rawcopyfsbackupjob.h"

const std::string suitePrefix = "RawCopyFsBackup/";

RawCopyFsBackupJobTest::RawCopyFsBackupJobTest(const std::string &dataPrefix,
                                               const std::string& errorPrefix)
    : AbstractCopyFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
{
}

RawCopyFsBackupJobTest::~RawCopyFsBackupJobTest()
{
}

AbstractCopyFsBackupJob *RawCopyFsBackupJobTest::CreateCopyJob()
{
    return new RawCopyFsBackupJob();
}

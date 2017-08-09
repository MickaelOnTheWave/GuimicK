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

// TODO : remove OBVIOUS duplication
AbstractCopyFsBackupJob *RawCopyFsBackupJobTest::CreateCopyJob()
{
    return new RawCopyFsBackupJob();
}

AbstractBackupJob *RawCopyFsBackupJobTest::CreateNewJob()
{
    return new RawCopyFsBackupJob();
}

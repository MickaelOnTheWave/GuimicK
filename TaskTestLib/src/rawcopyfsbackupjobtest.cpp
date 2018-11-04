#include "rawcopyfsbackupjobtest.h"

#include "oscopyfsbackupjob.h"

const std::string suitePrefix = "RawCopyFsBackup/";

RawCopyFsBackupJobTest::RawCopyFsBackupJobTest(const std::string &dataPrefix,
                                               const std::string& errorPrefix)
    : AbstractCopyFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
{
}

RawCopyFsBackupJobTest::~RawCopyFsBackupJobTest()
{
}

AbstractBackupJob *RawCopyFsBackupJobTest::CreateNewJob()
{
#ifdef _WIN32
   return new WindowsCopyFsBackupJob();
#else
   return new LinuxCopyFsBackupJob();
#endif
}

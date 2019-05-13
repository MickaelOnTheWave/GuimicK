#include "rawcopyfsbackupjobtest.h"

#include "oscopyfsbackupjob.h"

const std::wstring suitePrefix = L"RawCopyFsBackup/";

RawCopyFsBackupJobTest::RawCopyFsBackupJobTest(const std::wstring &dataPrefix,
                                               const std::wstring& errorPrefix)
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

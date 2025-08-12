#include "rsynccopyfsbackupjobtest.h"

#include "rsynccopyfsbackupjob.h"

const std::wstring suitePrefix = L"RsyncCopyFsBackup/";

RsyncCopyFsBackupJobTest::RsyncCopyFsBackupJobTest(const std::wstring &dataPrefix,
                                                   const std::wstring &errorPrefix)
    : AbstractCopyFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
{
}

RsyncCopyFsBackupJobTest::~RsyncCopyFsBackupJobTest()
{
}

AbstractCopyFsBackupJob *RsyncCopyFsBackupJobTest::CreateNewJob()
{
    return new RsyncCopyFsBackupJob();
}

#include "abstractcopyfsbackupjobtest.h"

AbstractCopyFsBackupJobTest::AbstractCopyFsBackupJobTest(const std::wstring &dataPrefix,
                                                         const std::wstring& errorPrefix)
 : AbstractFsBackupJobTest(dataPrefix, errorPrefix)
{
}

AbstractCopyFsBackupJobTest::~AbstractCopyFsBackupJobTest()
{
}

void AbstractCopyFsBackupJobTest::testRunBackup_data()
{
   LoadExternalDataSamples(false);
   LoadExternalDataSamples(true);
}

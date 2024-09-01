#include "abstractcopyfsbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"

using namespace std;

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

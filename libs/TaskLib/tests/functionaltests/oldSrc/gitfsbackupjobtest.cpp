#include "gitfsbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "gitfsbackupjob.h"

using namespace std;

const wstring suitePrefix = L"GitFsBackup/";

GitFsBackupJobTest::GitFsBackupJobTest(const wstring &dataPrefix,
                                       const wstring& errorPrefix)
    : AbstractFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
{
}

void GitFsBackupJobTest::testRunBackup_data()
{
    LoadExternalDataSamples(false);
    LoadExternalDataSamples(true);
}

AbstractBackupJob *GitFsBackupJobTest::CreateNewJob()
{
    return new GitFsBackupJob();
}

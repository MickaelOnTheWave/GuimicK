#include "gitfsbackupjobtest.h"

#include <QTest>

#include "gitfsbackupjob.h"

using namespace std;

const string suitePrefix = "GitFsBackup/";

GitFsBackupJobTest::GitFsBackupJobTest(const std::string &dataPrefix)
    : AbstractFsBackupJobTest(dataPrefix + suitePrefix)
{
}

GitFsBackupJobTest::~GitFsBackupJobTest()
{
}

void GitFsBackupJobTest::testRunBackup_data()
{
    LoadExternalDataSamples();
}

JobStatus *GitFsBackupJobTest::RunBackupJob()
{
    return nullptr;
}

void GitFsBackupJobTest::CheckBackedUpDataIsOk()
{
    QFAIL("TODO : implement");
}

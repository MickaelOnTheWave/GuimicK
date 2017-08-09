#include "gitfsbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"

using namespace std;

const string suitePrefix = "GitFsBackup/";
const string repository = "repository/";
const string restoredBackup = "restore/";
const string sshUser = "mickael";
const string sshHost = "192.168.1.101";


GitFsBackupJobTest::GitFsBackupJobTest(const std::string &dataPrefix,
                                       const std::string& errorPrefix)
    : AbstractFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
{
}

GitFsBackupJobTest::~GitFsBackupJobTest()
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

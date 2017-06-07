#include "gitfsbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "gitfsbackupjob.h"

using namespace std;

const string suitePrefix = "GitFsBackup/";
const string repository = "repository/";
const string restoredBackup = "restore/";
const string sshUser = "mickael";
const string sshHost = "192.168.1.101";


GitFsBackupJobTest::GitFsBackupJobTest(const std::string &dataPrefix)
    : AbstractFsBackupJobTest(dataPrefix + suitePrefix)
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

JobStatus *GitFsBackupJobTest::RunBackupJob()
{
    QFETCH(bool, remote);

    GitFsBackupJob* job = new GitFsBackupJob();
    job->SetForceRawCopyUse(true);
    job->AddFolder(FileTools::BuildFullPath(currentSourceFolder), repository);
    if (remote)
        job->SetTargetRemote(sshUser, sshHost);
    else
        job->SetTargetLocal();

    JobStatus* status = job->Run();

    delete job;
    return status;
}

void GitFsBackupJobTest::CheckBackedUpDataIsOk()
{
    FileTestUtils::CheckFoldersHaveSameContent(repository, currentSourceFolder);
}


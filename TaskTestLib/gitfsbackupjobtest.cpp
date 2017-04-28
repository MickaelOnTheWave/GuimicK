#include "gitfsbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "gitfsbackupjob.h"

using namespace std;

const string suitePrefix = "GitFsBackup/";
const string repository = "repository/";
const string restoredBackup = "restore/";

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
    GitFsBackupJob* job = new GitFsBackupJob();
    job->AddFolder(FileTools::BuildFullPath(currentSourceFolder), repository);
    JobStatus* status = job->Run();

    delete job;
    return status;
}

void GitFsBackupJobTest::CheckBackedUpDataIsOk()
{
    FileTestUtils::CheckFoldersHaveSameContent(repository, currentSourceFolder);
}


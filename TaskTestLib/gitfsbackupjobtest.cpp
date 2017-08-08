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

void GitFsBackupJobTest::testCreatesOnlyOneAttachment()
{
    vector<string> expectedAttachments = {"Git Filesystem Backup.txt"};
    testCheckJobAttachments(false, expectedAttachments);
}

void GitFsBackupJobTest::testCreatesDebugAttachment()
{
    vector<string> expectedAttachments =
    {
        "Git Filesystem Backup.txt",
        "Git Filesystem Backup - Debug.txt",
    };

    testCheckJobAttachments(true, expectedAttachments);
}

JobStatus *GitFsBackupJobTest::RunBackupJob()
{
    QFETCH(bool, remote);

    GitFsBackupJob* job = new GitFsBackupJob();
    JobStatus* status = RunBackupJob(job, remote);
    delete job;
    return status;
}

void GitFsBackupJobTest::testCheckJobAttachments(const bool debugOutput,
                                                 const std::vector<string> &expectedAttachments)
{
    GitFsBackupJob* job = new GitFsBackupJob();
    job->SetOutputDebugInformation(debugOutput);
    job->AddFolder("dummySource", "dummyDestination");
    JobStatus* status = RunBackupJob(job, false);

    JobStatus::FileBufferList fileBuffers;
    status->GetFileBuffers(fileBuffers);
    QCOMPARE(fileBuffers.size(), expectedAttachments.size());

    vector<string> attachmentNames;
    for (auto&& it : fileBuffers)
        attachmentNames.push_back(it.first);

    bool isEqual = std::equal(attachmentNames.begin(), attachmentNames.end(),
                              expectedAttachments.begin());
    QCOMPARE(isEqual, true);

    delete status;
    delete job;
}

JobStatus *GitFsBackupJobTest::RunBackupJob(GitFsBackupJob* job, const bool isRemote)
{
    job->AddFolder(FileTools::BuildFullPath(currentSourceFolder), repository);
    if (isRemote)
        job->SetTargetRemote(sshUser, sshHost);
    else
        job->SetTargetLocal();

    return job->Run();
}

void GitFsBackupJobTest::CheckBackedUpDataIsOk()
{
    FileTestUtils::CheckFoldersHaveSameContent(repository, currentSourceFolder);
}


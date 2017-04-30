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
    LoadCustomDataSamples(false);
    LoadCustomDataSamples(true);
}

void GitFsBackupJobTest::LoadCustomDataSamples(const bool isRemote)
{
    QTest::addColumn<QString>("sourceBefore");
    QTest::addColumn<QString>("sourceNow");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QString>("report");
    QTest::addColumn<bool>("remote");

    QStringList testCases = FileTestUtils::GetFolderList(GetDataFolder().c_str());
    for (auto it=testCases.begin(); it!=testCases.end(); ++it)
    {
        string stdString = it->toStdString();
        QTest::newRow(stdString.c_str())
                                << "sourceBefore"
                                << "sourceNow"
                                << "miniDescription.txt"
                                << "fullReport.txt"
                                << isRemote;
    }
}

JobStatus *GitFsBackupJobTest::RunBackupJob()
{
    QFETCH(bool, remote);

    GitFsBackupJob* job = new GitFsBackupJob();
    job->AddFolder(FileTools::BuildFullPath(currentSourceFolder), repository);
    if (remote)
        job->SetTargetRemote(sshUser, sshHost);

    JobStatus* status = job->Run();

    delete job;
    return status;
}

void GitFsBackupJobTest::CheckBackedUpDataIsOk()
{
    FileTestUtils::CheckFoldersHaveSameContent(repository, currentSourceFolder);
}


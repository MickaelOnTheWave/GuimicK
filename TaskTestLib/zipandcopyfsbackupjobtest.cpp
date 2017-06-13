#include "zipandcopyfsbackupjobtest.h"

#include <QTest>
#include "filetestutils.h"
#include "filetools.h"
#include "zipandcopyfsbackupjob.h"

using namespace std;

const string suitePrefix = "ZipAndCopyFsBackup/";
const string sshUser = "mickael";
const string sshHost = "192.168.1.101";
const string repository = "repository";
const string restoredFolder = "restore";


ZipAndCopyFsBackupJobTest::ZipAndCopyFsBackupJobTest(const std::string &dataPrefix)
    : AbstractFsBackupJobTest(dataPrefix + suitePrefix)
{
}

ZipAndCopyFsBackupJobTest::~ZipAndCopyFsBackupJobTest()
{
}

void ZipAndCopyFsBackupJobTest::testRunBackup_data()
{
    LoadExternalDataSamples(false);
    LoadExternalDataSamples(true);
}

void ZipAndCopyFsBackupJobTest::CheckBackedUpDataIsOk()
{
    bool ok = ZipAndCopyFsBackupJob::Restore(repository, restoredFolder);
    QCOMPARE(ok, true);

    FileTestUtils::CheckFoldersHaveSameContent(currentSourceFolder, restoredFolder);
}

JobStatus *ZipAndCopyFsBackupJobTest::RunBackupJob()
{
    QFETCH(bool, remote);

    ZipAndCopyFsBackupJob* job = new ZipAndCopyFsBackupJob();
    job->AddFolder(FileTools::BuildFullPath(currentSourceFolder), repository);
    if (remote)
        job->SetTargetRemote(sshUser, sshHost);
    else
        job->SetTargetLocal();

    JobStatus* status = job->Run();

    delete job;
    return status;
}

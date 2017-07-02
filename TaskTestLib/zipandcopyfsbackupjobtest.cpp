#include "zipandcopyfsbackupjobtest.h"

#include <QTest>
#include "filetestutils.h"
#include "filetools.h"
#include "zipandcopyfsbackupjob.h"

using namespace std;

const string suitePrefix = "ZipAndCopyFsBackup/";
const string sshUser = "mickael";
const string sshHost = "192.168.1.101";
const string restoredFolder = "restore";
const string archiveName = "backupArchive.tar";

ZipAndCopyFsBackupJobTest::ZipAndCopyFsBackupJobTest(const string &dataPrefix,
                                                     const string& errorPrefix)
    : AbstractFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
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
    bool ok = ZipAndCopyFsBackupJob::Restore(archiveName, restoredFolder);
    QCOMPARE(ok, true);

    FileTestUtils::CheckFoldersHaveSameContent(currentSourceFolder, restoredFolder);
}

JobStatus *ZipAndCopyFsBackupJobTest::RunBackupJob()
{
    QFETCH(bool, remote);

    ZipAndCopyFsBackupJob* job = new ZipAndCopyFsBackupJob();
    job->AddFolder(FileTools::BuildFullPath(currentSourceFolder), archiveName);
    if (remote)
        job->SetTargetRemote(sshUser, sshHost);
    else
        job->SetTargetLocal();

    JobStatus* status = job->Run();

    delete job;
    return status;
}

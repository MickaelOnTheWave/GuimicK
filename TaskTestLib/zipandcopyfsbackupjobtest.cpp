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
const string localArchive = "/tmp/localBackupArchive.tar";

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

JobStatus *ZipAndCopyFsBackupJobTest::RunBackupJob(const bool isRemote,
                                                   const bool useDebug)
{
    // TODO : improve this. There are only two lines that are specific to this test suite,
    // the rest of the code should be used from parent class.
    ZipAndCopyFsBackupJob* job = new ZipAndCopyFsBackupJob();
    job->InitializeFromClient(nullptr);
    job->AddFolder(FileTools::BuildFullPath(currentSourceFolder), archiveName);

    job->SetLocalDestination(localArchive);

    if (isRemote)
        job->SetTargetRemote(sshUser, sshHost);
    else
        job->SetTargetLocal();

    job->SetOutputDebugInformation(useDebug ? DebugOutput::ALWAYS : DebugOutput::NEVER);

    JobStatus* status = job->Run();

    delete job;
    return status;
}

AbstractBackupJob *ZipAndCopyFsBackupJobTest::CreateNewJob()
{
    return new ZipAndCopyFsBackupJob();
}

#include "zipandcopyfsbackupjobtest.h"

#include <QTest>
#include "filetestutils.h"
#include "filetools.h"
#include "pathtools.h"
#include "tarbackupjob.h"

using namespace std;

namespace
{
   const wstring suitePrefix = L"ZipAndCopyFsBackup/";
   const wstring sshUser = L"mickael";
   const wstring sshHost = L"192.168.1.101";
   const wstring restoredFolder = L"restore";
   const wstring archiveName = L"backupArchive.tar";
   const wstring localArchive = L"/tmp/localBackupArchive.tar";
}

ZipAndCopyFsBackupJobTest::ZipAndCopyFsBackupJobTest(const wstring& dataPrefix,
                                                     const wstring& errorPrefix)
    : AbstractFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
{
}

void ZipAndCopyFsBackupJobTest::testRunBackup_data()
{
    LoadExternalDataSamples(false);
    LoadExternalDataSamples(true);
}

void ZipAndCopyFsBackupJobTest::CheckBackedUpDataIsOk()
{
    bool ok = TarBackupJob::Restore(archiveName, restoredFolder);
    QCOMPARE(ok, true);

    FileTestUtils::CheckFoldersHaveSameContent(
             QString::fromStdWString(currentSourceFolder),
             QString::fromStdWString(restoredFolder));
}

JobStatus *ZipAndCopyFsBackupJobTest::RunBackupJob(const bool isRemote,
                                                   const bool useDebug)
{
    // TODO : improve this. There are only two lines that are specific to this test suite,
    // the rest of the code should be used from parent class.
    TarBackupJob* job = new TarBackupJob();
    job->InitializeFromClient(nullptr);
    job->AddFolder(PathTools::BuildFullPathIfRelative(currentSourceFolder), archiveName);

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

wstring ZipAndCopyFsBackupJobTest::GetBackupDestination() const
{
   return L"SingleFolder.tar";
}

AbstractBackupJob *ZipAndCopyFsBackupJobTest::CreateNewJob()
{
    return new TarBackupJob();
}

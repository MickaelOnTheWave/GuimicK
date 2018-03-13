#include "tarincrementalbackupjobtest.h"

#include <unistd.h>
#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "tarincrementalbackup.h"
#include "tartools.h"

using namespace std;

const string suitePrefix = "TarIncrementalBackup/";
const string archiveName = "backupArchive.tar";
const string restoredFolder = "restore";

TarIncrementalBackupJobTest::TarIncrementalBackupJobTest(
      const string& dataPrefix, const string& errorPrefix)
   : AbstractFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
{
}

void TarIncrementalBackupJobTest::testRunBackup_data()
{
   LoadExternalDataSamples(false);
}

void TarIncrementalBackupJobTest::ProcessingBetweenBackups()
{
    //sleep(2);
}

void TarIncrementalBackupJobTest::CheckBackedUpDataIsOk()
{
   TarTools tarTool;
   bool ok = tarTool.ExtractArchive(archiveName, restoredFolder);
   QCOMPARE(ok, true);

   FileTestUtils::CheckFoldersHaveSameContent(currentSourceFolder, restoredFolder);
}

JobStatus *TarIncrementalBackupJobTest::RunBackupJob( const bool isRemote,
                                                      const bool useDebug)
{
    // TODO : improve this. There are only two lines that are specific to this test suite,
    // the rest of the code should be used from parent class.
    TarIncrementalBackup* job = new TarIncrementalBackup();
    job->InitializeFromClient(nullptr);
    job->AddFolder(FileTools::BuildFullPathIfRelative(currentSourceFolder), archiveName);

    //job->SetLocalDestination(localArchive);

    /*if (isRemote)
        job->SetTargetRemote(sshUser, sshHost);
    else*/
        job->SetTargetLocal();

    job->SetOutputDebugInformation(useDebug ? DebugOutput::ALWAYS : DebugOutput::NEVER);

    JobStatus* status = job->Run();

    delete job;
    return status;
}

string TarIncrementalBackupJobTest::GetBackupDestination() const
{
   return "SingleFolder.tar";
}

AbstractBackupJob* TarIncrementalBackupJobTest::CreateNewJob()
{
   return new TarIncrementalBackup();
}

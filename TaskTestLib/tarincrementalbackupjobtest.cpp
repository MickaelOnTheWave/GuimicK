#include "tarincrementalbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "tarincrementalbackup.h"
#include "tartools.h"

using namespace std;

const string folderToBackup = "folderToBackup";
const string archiveName = "backupArchive.tar";

TarIncrementalBackupJobTest::TarIncrementalBackupJobTest(
      const string& dataPrefix, const string& errorPrefix)
   : QtTestSuite(dataPrefix, errorPrefix)
{
}

void TarIncrementalBackupJobTest::testRunBackup_Added()
{
   auto backupJob = CreateInitializedJob();
   CreateInitialData();
   RunInitialBackup(backupJob);

   // Change data
   QStringList filesToAdd = {"added0.txt", "added1.txt"};


   JobStatus* status = backupJob->Run();
   QVERIFY(status->GetCode() == JobStatus::OK);
   // Check that report is as expected
   QFAIL("Implementation not finished");

   delete backupJob;
}

void TarIncrementalBackupJobTest::testRunBackup_Modified()
{
   QFAIL("Not Implemented yet");

}

void TarIncrementalBackupJobTest::testRunBackup_Mixed()
{
   QFAIL("Not Implemented yet");
}

void TarIncrementalBackupJobTest::CreateInitialData()
{
   QStringList initialFiles = {
      "initialFile01.txt", "initialFile02.txt", "initialFile03.txt"
   };
   FileTestUtils::CreatePopulatedFolder(folderToBackup, initialFiles);
}

void TarIncrementalBackupJobTest::RunInitialBackup(AbstractBackupJob* job)
{
   JobStatus* status = job->Run();
   QVERIFY(status->GetCode() == JobStatus::OK);
   // Check added data
   delete status;
}

AbstractBackupJob* TarIncrementalBackupJobTest::CreateInitializedJob()
{
   auto job = new TarIncrementalBackup();
   job->InitializeFromClient(nullptr);
   job->AddFolder(FileTools::BuildFullPathIfRelative(folderToBackup), archiveName);
   job->SetTargetLocal();
   job->SetOutputDebugInformation(DebugOutput::NEVER);
   return job;
}

/*
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
    else*
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
}*/

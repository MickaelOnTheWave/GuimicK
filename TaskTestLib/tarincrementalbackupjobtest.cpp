#include "tarincrementalbackupjobtest.h"

#include <memory>
#include <QTest>
#include <unistd.h>

#include "filetestutils.h"
#include "filetools.h"
#include "tarincrementalbackup.h"
#include "tartools.h"
#include "tools.h"

using namespace std;

const string folderToBackup = "folderToBackup";
const string archiveName = "backupArchive.tar";

TarIncrementalBackupJobTest::TarIncrementalBackupJobTest(
      const string& dataPrefix, const string& errorPrefix)
   : QtTestSuite(dataPrefix, errorPrefix)
{
}

void TarIncrementalBackupJobTest::init()
{
    cleanup();
}

void TarIncrementalBackupJobTest::cleanup()
{
    string unusedOutput;
    Tools::RunExternalCommandToBuffer("rm -Rf *", unusedOutput, true);
}

void TarIncrementalBackupJobTest::testRunBackup_Added()
{
   const QStringList filesToAdd = {"added0.txt", "added1.txt"};
   testRunBackup(filesToAdd, QStringList());
}

void TarIncrementalBackupJobTest::testRunBackup_Modified()
{
   const QStringList toModify({"initialFile01.txt", "initialFile02.txt"});
   testRunBackup(QStringList(), toModify);
}

void TarIncrementalBackupJobTest::testRunBackup_Mixed()
{
   const QStringList filesToAdd = {"mixAdd0.txt", "mixAdd1.txt", "mixAdd2.txt"};
   const QStringList filesToModify = {"initialFile01.txt"};
   testRunBackup(filesToAdd, filesToModify);
}

void TarIncrementalBackupJobTest::testBackupAndRestoreMultipleLevels()
{
   auto backupJob = CreateInitializedJob();
   CreateInitialData();
   RunInitialBackup(backupJob);

   const QStringList addStage1 = {"add0St1.txt", "add1St1.txt"};
   RunBackupStageWithoutStatus(backupJob, addStage1, QStringList());

   const QStringList modifyStage2 = {"initial01.txt", "add1St1.txt"};
   RunBackupStageWithoutStatus(backupJob, QStringList(), modifyStage2);

   const QStringList addStage3 = {"add0St3.txt"};
   const QStringList modifyStage3 = {"initial01.txt", "add0St1.txt"};
   RunBackupStageWithoutStatus(backupJob, addStage3, modifyStage3);

   QFAIL("Implement Restore tests");
}

void TarIncrementalBackupJobTest::testRunBackup(const QStringList& toAdd, const QStringList& toModify)
{
   auto backupJob = CreateInitializedJob();
   CreateInitialData();
   RunInitialBackup(backupJob);

   JobStatus* status = RunBackupStage(backupJob, toAdd, toModify);
   CheckStatus(status, toAdd, toModify);

   delete backupJob;
}

JobStatus* TarIncrementalBackupJobTest::RunBackupStage(AbstractBackupJob* job,
                                                       const QStringList& toAdd,
                                                       const QStringList& toModify)
{
   AddFiles(toAdd);
   ModifyFiles(toModify);
   return job->Run();
}

void TarIncrementalBackupJobTest::RunBackupStageWithoutStatus(AbstractBackupJob* job,
                                                              const QStringList& toAdd,
                                                              const QStringList& toModify)
{
   JobStatus* status = RunBackupStage(job, toAdd, toModify);
   delete status;
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

void TarIncrementalBackupJobTest::AddFiles(const QStringList& filesToAdd)
{
   for (auto it : filesToAdd)
   {
      const string currentFile = folderToBackup + "/" + it.toStdString();
      FileTools::WriteBufferToFile(currentFile, "lalala");
   }
}

void TarIncrementalBackupJobTest::ModifyFiles(const QStringList& filesToModify)
{
   sleep(2);
   for (auto it : filesToModify)
   {
      const string currentFile = folderToBackup + "/" + it.toStdString();
      FileTools::WriteBufferToFile(currentFile, "lololo");
   }
}

void TarIncrementalBackupJobTest::CheckStatus(JobStatus* status,
                                              const QStringList& addedFiles,
                                              const QStringList& modifiedFiles)
{
   QVERIFY(status->GetCode() == JobStatus::OK);

   FileBackupReport expectedReport;
   for (auto it : addedFiles)
      expectedReport.AddAsAdded(it.toStdString());
   for (auto it : modifiedFiles)
      expectedReport.AddAsModified(it.toStdString());
   CheckReport(status, expectedReport);
}

void TarIncrementalBackupJobTest::CheckReport(JobStatus* status,
                                              const FileBackupReport& expectedReport)
{
   JobStatus::FileBufferList attachments;
   status->GetFileBuffers(attachments);
   QCOMPARE(attachments.size(), 1ul);
   if (attachments.front().second != expectedReport.GetFullDescription())
   {
      FileTools::WriteBufferToFile(GetErrorFolder()+"ExpectedReport", expectedReport.GetFullDescription());
      FileTools::WriteBufferToFile(GetErrorFolder()+"ResultedReport", attachments.front().second);
      QFAIL("Report different than expected");
   }
}


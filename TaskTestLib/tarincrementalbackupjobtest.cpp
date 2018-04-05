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

const QStringList initialTestFiles = {
   "initialFile01.txt", "initialFile02.txt", "initialFile03.txt"
};
const QStringList initialTestFilesContents = {
   "woo\nhoo", "waaaaahaaaaa", "wiiiihiiiiii"
};


TarIncrementalBackupJobTest::TarIncrementalBackupJobTest()
   : QtTestSuite("", "")
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
   const QStringList contentsToAdd = {"addContent0", "add content for file 1"};
   testRunBackup(filesToAdd, contentsToAdd, QStringList(), QStringList());
}

void TarIncrementalBackupJobTest::testRunBackup_Modified()
{
   const QStringList names({"initialFile01.txt", "initialFile02.txt"});
   const QStringList contents({"\nappend content to 1", "\nappend more content to file 2"});
   testRunBackup(QStringList(), QStringList(), names, contents);
}

void TarIncrementalBackupJobTest::testRunBackup_Mixed()
{
   const QStringList addNames = {"mixAdd0.txt", "mixAdd1.txt", "mixAdd2.txt"};
   const QStringList addContents = {"cont0", "cont1", "cont2"};
   const QStringList modifyNames = {"initialFile01.txt"};
   const QStringList modifyContents = {"\nhopefully this has been added\n"};
   testRunBackup(addNames, addContents, modifyNames, modifyContents);
}

void TarIncrementalBackupJobTest::testBackupAndRestoreMultipleLevels()
{
   auto backupJob = CreateInitializedJob();
   CreateInitialData();
   RunInitialBackup(backupJob);

   const QStringList addStage1 = {"add0St1.txt", "add1St1.txt"};
   const QStringList addContentStage1 = {"blablabla1", "blobloblo2"};
   RunBackupStageWithoutStatus(backupJob, addStage1, addContentStage1,
                               QStringList(), QStringList());

   const QStringList modifyStage2 = {"initialFile01.txt", "add1St1.txt"};
   const QStringList modifyContentStage2 = {"woo\nhaaaahaaaioiljk", "mymodifiedcontent"};
   RunBackupStageWithoutStatus(backupJob, QStringList(), QStringList(),
                               modifyStage2, modifyContentStage2);

   const QStringList addStage3 = {"add0St3.txt"};
   const QStringList addContentStage3 = {"mynewcontent"};
   const QStringList modifyStage3 = {"initialFile01.txt", "add0St1.txt"};
   const QStringList modifyContentStage3 = {"\nchange me again", "\ni want some changes too"};
   RunBackupStageWithoutStatus(backupJob, addStage3, addContentStage3,
                               modifyStage3, modifyContentStage3);

   RestoreAndCheck(backupJob, 3, initialTestFiles, initialTestFilesContents);

   const QStringList stage1Files = {"add0St1.txt", "add1St1.txt"};
   RestoreAndCheck(backupJob, 2, initialTestFiles + stage1Files,
                   initialTestFilesContents + addContentStage1);

   QStringList contentStage2 = initialTestFilesContents + addContentStage1;
   contentStage2[0] += modifyContentStage2[0];
   contentStage2[4] += modifyContentStage2[1];
   RestoreAndCheck(backupJob, 1, initialTestFiles + stage1Files,
                   contentStage2);

   const QStringList stage3Files = {"add0St1.txt", "add1St1.txt", "add0St3.txt"};
   QStringList contentStage3 = contentStage2;
   contentStage3.append(addContentStage3.first());
   contentStage3[0] += modifyContentStage3[0];
   contentStage3[3] += modifyContentStage3[1];
   RestoreAndCheck(backupJob, 0, initialTestFiles + stage3Files,
                   contentStage3);
}

void TarIncrementalBackupJobTest::testRunBackup(const QStringList& addNames,
                                                const QStringList& addContents,
                                                const QStringList& modifyNames,
                                                const QStringList& modifyContents)
{
   auto backupJob = CreateInitializedJob();
   CreateInitialData();
   RunInitialBackup(backupJob);

   JobStatus* status = RunBackupStage(backupJob, addNames, addContents,
                                      modifyNames, modifyContents);
   CheckStatus(status, addNames, modifyNames);

   delete backupJob;
}

JobStatus* TarIncrementalBackupJobTest::RunBackupStage(AbstractBackupJob* job,
                                                       const QStringList& addNames,
                                                       const QStringList& addContents,
                                                       const QStringList& modifyNames,
                                                       const QStringList& modifyContents)
{
   AddFiles(addNames, addContents);
   ModifyFiles(modifyNames, modifyContents);
   return job->Run();
}

void TarIncrementalBackupJobTest::RunBackupStageWithoutStatus(AbstractBackupJob* job,
                                                              const QStringList& addNames,
                                                              const QStringList& addContents,
                                                              const QStringList& modifyNames,
                                                              const QStringList& modifyContents)
{
   JobStatus* status = RunBackupStage(job, addNames, addContents,
                                      modifyNames, modifyContents);
   delete status;
}

void TarIncrementalBackupJobTest::CreateInitialData()
{
   FileTools::CreateFolder(folderToBackup);

   auto itName = initialTestFiles.begin();
   auto itContent = initialTestFilesContents.begin();
   for (; itName != initialTestFiles.end(); ++itName, ++itContent)
   {
       const string filename = folderToBackup + "/" + itName->toStdString();
       FileTools::WriteBufferToFile(filename, itContent->toStdString());
   }
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

void TarIncrementalBackupJobTest::AddFiles(const QStringList& names,
                                           const QStringList& contents)
{
   auto itName = names.begin();
   auto itContent = contents.begin();
   for (; itName != names.end(); ++itName, ++itContent)
   {
      const string currentFile = folderToBackup + "/" + itName->toStdString();
      FileTools::WriteBufferToFile(currentFile, itContent->toStdString());
   }
}

void TarIncrementalBackupJobTest::ModifyFiles(const QStringList& names,
                                              const QStringList& contents)
{
   sleep(2);
   auto itName = names.begin();
   auto itContent = contents.begin();
   for (; itName != names.end(); ++itName, ++itContent)
   {
      const string currentFile = folderToBackup + "/" + itName->toStdString();
      const string currentContent = FileTools::GetTextFileContent(currentFile);
      FileTools::WriteBufferToFile(currentFile, currentContent + itContent->toStdString());
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

void TarIncrementalBackupJobTest::RestoreAndCheck(AbstractBackupJob* job, const int timeIndex,
                                                  const QStringList& expectedFiles,
                                                  const QStringList& expectedContents)
{
   const string restoreFolder = "Restore";
   FileTools::CreateFolder(restoreFolder);

   JobStatus* status = job->RestoreBackupFromServer(restoreFolder, 0, timeIndex);
   QCOMPARE(status->GetCode(), JobStatus::OK);
   FileTestUtils::CheckFolderContent(restoreFolder, expectedFiles, expectedContents);

   FileTools::RemoveFolder(restoreFolder, false);
}


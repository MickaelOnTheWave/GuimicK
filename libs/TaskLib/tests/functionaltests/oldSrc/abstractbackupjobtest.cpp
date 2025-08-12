#include "abstractbackupjobtest.h"

#include <catch2/catch.hpp>

#include "filetestutils.h"
#include "filetools.h"
#include "tools.h"

using namespace std;

AbstractBackupJobTest::AbstractBackupJobTest(
      const wstring& dataPrefix,
      const wstring& errorPrefix
   ) :
  dataFolderPrefix(dataPrefix)
{
}

void AbstractBackupJobTest::TestBackupAndRestore()
{
   auto testFunction = [this](const wstring& sourceBefore, const wstring& sourceNow, const bool remote)
   {
      const wstring restoreFolder = L"restore/";

      AbstractBackupJob* job = CreateNewJob();
      RunBackup(job, sourceNow);
      RunRestore(job, restoreFolder);
      const bool same = FileTestUtils::CheckFoldersHaveSameContent(
          QString::fromStdWString(sourceNow),
          QString::fromStdWString(restoreFolder)
          );
      REQUIRE(same == true);
      delete job;
   };
   TestOnDataset(testFunction);
}

void AbstractBackupJobTest::TestOnDataset(TestFunction testFunction)
{
   const QStringList testCases = FileTestUtils::GetFolderList(QString::fromStdWString(dataFolderPrefix));
   for (const auto& testCase : testCases)
   {
      currentTestCaseFolder = dataFolderPrefix + testCase.toStdWString() + L"/";

      const wstring beforeFolder = currentTestCaseFolder + L"sourceBefore";
      const wstring nowFolder = currentTestCaseFolder + L"sourceNow";

      testFunction(beforeFolder, nowFolder, true);
   }
}

void AbstractBackupJobTest::init()
{
    cleanup();
}

void AbstractBackupJobTest::cleanup()
{
    wstring unusedOutput;
    Tools::RunExternalCommandToBuffer(L"rm -Rf *", unusedOutput, true);
}

void AbstractBackupJobTest::RunBackup(AbstractBackupJob* job, const wstring& folder)
{
   PrepareBackup(job, folder);
   job->SetTargetLocal();
   JobStatus* status = job->Run();
   REQUIRE(status->GetCode() == JobStatus::Ok);
   delete status;
}

void AbstractBackupJobTest::RunRestore(AbstractBackupJob* job, const wstring& folder)
{
   REQUIRE(FileTools::CreateFolder(folder) == true);
   JobStatus* status = job->RestoreBackupFromServer(folder);
   REQUIRE(status->GetCode() == JobStatus::Ok);
   delete status;
}

void AbstractBackupJobTest::PrepareBackup(AbstractBackupJob* job,
                                          const wstring& folder)
{
   REQUIRE(FileTools::CreateFolder(backupRepository) == true);
   currentSourceFolder = folder;
   job->SetRepository(backupRepository);
   job->AddFolder(currentSourceFolder, GetBackupDestination());
}

wstring AbstractBackupJobTest::GetBackupDestination() const
{
   return wstring(L"SingleFolder");
}

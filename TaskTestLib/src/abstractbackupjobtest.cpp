#include "abstractbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "stringtools.h"
#include "tools.h"

using namespace std;

AbstractBackupJobTest::AbstractBackupJobTest(
      const wstring& dataPrefix,
      const wstring& errorPrefix
   ) : QtTestSuite(dataPrefix, errorPrefix)
{
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

void AbstractBackupJobTest::testBackupAndRestore_data()
{
   LoadExternalDataSamples(false);
}

void AbstractBackupJobTest::testBackupAndRestore()
{
   QFETCH(QString, sourceNow);
   currentTestCaseName = StringTools::Utf8ToUnicode(QTest::currentDataTag());
   currentTestCaseFolder = GetDataFolder() + currentTestCaseName + L"/";

   const wstring sourceFolder = currentTestCaseFolder + sourceNow.toStdWString();
   const wstring restoreFolder = L"restore/";

   AbstractBackupJob* job = CreateNewJob();
   RunBackup(job, sourceFolder);
   RunRestore(job, restoreFolder);
   FileTestUtils::CheckFoldersHaveSameContent(
            QString::fromStdWString(sourceFolder),
            QString::fromStdWString(restoreFolder));
   delete job;
}

void AbstractBackupJobTest::LoadExternalDataSamples(const bool isRemote)
{
    QTest::addColumn<QString>("sourceBefore");
    QTest::addColumn<QString>("sourceNow");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QString>("report");
    QTest::addColumn<bool>("remote");

    QStringList testCases = FileTestUtils::GetFolderList(QString::fromStdWString(GetDataFolder()));
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

void AbstractBackupJobTest::RunBackup(AbstractBackupJob* job, const wstring& folder)
{
   PrepareBackup(job, folder);
   job->SetTargetLocal();
   JobStatus* status = job->Run();
   QCOMPARE(status->GetCode(), JobStatus::Ok);
   delete status;
}

void AbstractBackupJobTest::RunRestore(AbstractBackupJob* job, const wstring& folder)
{
   QCOMPARE(FileTools::CreateFolder(folder), true);
   JobStatus* status = job->RestoreBackupFromServer(folder);
   QCOMPARE(status->GetCode(), JobStatus::Ok);
   delete status;
}

void AbstractBackupJobTest::PrepareBackup(AbstractBackupJob* job,
                                          const wstring& folder)
{
   QCOMPARE(FileTools::CreateFolder(backupRepository), true);
   currentSourceFolder = folder;
   job->SetRepository(backupRepository);
   job->AddFolder(currentSourceFolder, GetBackupDestination());
}

wstring AbstractBackupJobTest::GetBackupDestination() const
{
   return wstring(L"SingleFolder");
}

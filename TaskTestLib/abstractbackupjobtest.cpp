#include "abstractbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "tools.h"

using namespace std;

AbstractBackupJobTest::AbstractBackupJobTest(
   const string &dataPrefix,
   const string &errorPrefix
   ) : QtTestSuite(dataPrefix, errorPrefix)
{
}

void AbstractBackupJobTest::init()
{
    cleanup();
}

void AbstractBackupJobTest::cleanup()
{
    string unusedOutput;
    Tools::RunExternalCommandToBuffer("rm -Rf *", unusedOutput, true);
}

void AbstractBackupJobTest::testBackupAndRestore_data()
{
   LoadExternalDataSamples(false);
}

void AbstractBackupJobTest::testBackupAndRestore()
{
   QFETCH(QString, sourceNow);
   currentTestCaseName = QTest::currentDataTag();
   currentTestCaseFolder = GetDataFolder() + currentTestCaseName + "/";

   const string sourceFolder = currentTestCaseFolder + sourceNow.toStdString();
   const string restoreFolder = "restore/";

   AbstractBackupJob* job = CreateNewJob();
   RunBackup(job, sourceFolder);
   RunRestore(job, restoreFolder);
   FileTestUtils::CheckFoldersHaveSameContent(sourceFolder, restoreFolder);
   delete job;
}

void AbstractBackupJobTest::LoadExternalDataSamples(const bool isRemote)
{
    QTest::addColumn<QString>("sourceBefore");
    QTest::addColumn<QString>("sourceNow");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QString>("report");
    QTest::addColumn<bool>("remote");

    QStringList testCases = FileTestUtils::GetFolderList(GetDataFolder().c_str());
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

void AbstractBackupJobTest::RunBackup(AbstractBackupJob* job, const string& folder)
{
   PrepareBackup(job, folder);
   job->SetTargetLocal();
   JobStatus* status = job->Run();
   QCOMPARE(status->GetCode(), JobStatus::OK);
   delete status;
}

void AbstractBackupJobTest::RunRestore(AbstractBackupJob* job, const string& folder)
{
   QCOMPARE(FileTools::CreateFolder(folder), true);
   JobStatus* status = job->RestoreBackup(folder);
   QCOMPARE(status->GetCode(), JobStatus::OK);
   delete status;
}

void AbstractBackupJobTest::PrepareBackup(AbstractBackupJob* job,
                                          const string& folder)
{
   QCOMPARE(FileTools::CreateFolder(backupRepository), true);
   QCOMPARE(FileTools::CreateFolder(currentSourceFolder), true);
   FileTestUtils::CopyFolderContent(folder, currentSourceFolder);
   job->SetRepository(backupRepository);
   job->AddFolder(currentSourceFolder, GetBackupDestination());
}

string AbstractBackupJobTest::GetBackupDestination() const
{
   return string("SingleFolder");
}

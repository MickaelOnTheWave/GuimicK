#include "rsnapshotjobtest.h"

#include <QDir>
#include <QTest>

#include <sstream>
#include <unistd.h>

#include "filetestutils.h"
#include "filetools.h"
#include "rsnapshotsmartbackupjob.h"
#include "rsnapshotconfigurationbuilder.h"
#include "tools.h"

using namespace std;

const wstring templateConfigurationFile = L"rsnapshot.conf";
const wstring suiteFolder = L"Rsnapshot/";

RsnapshotJobTest::RsnapshotJobTest(const wstring& dataPrefix, const wstring& errorPrefix)
    : AbstractFsBackupJobTest(dataPrefix + suiteFolder, errorPrefix)
{
}

RsnapshotJobTest::~RsnapshotJobTest()
{
}

void RsnapshotJobTest::testCreate_InvalidSource()
{
    AbstractBackupJob* job = CreateNewJob();
    job->AddFolder(L"dummySource", L"dummyName");
    JobStatus* status = job->Run();
    delete job;

    QCOMPARE(status->GetCode(), JobStatus::Error);
    const wstring expectedMessage = L"Tried to backup invalid folder";
    QCOMPARE(status->GetDescription(), expectedMessage);

    vector<pair<wstring,wstring> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.size(), 0ul);

    delete status;
}

void RsnapshotJobTest::testRunBackup_data()
{
    LoadExternalDataSamples(false);
}

void RsnapshotJobTest::testSmartCreator_TempFileIsCleaned()
{
    unsigned int beforeFileCount = GetFileNumberInCurrentFolder();
    QCOMPARE(beforeFileCount, 0u);

    JobStatus* status = RunRsnapshotJob();
    delete status;

    unsigned int afterFileCount = GetFileNumberInCurrentFolder();
    QCOMPARE(afterFileCount, beforeFileCount);
}

void RsnapshotJobTest::testSmartCreator_TempFileDoesNotOverwrite()
{
    const wstring dummyContent = L"First line\nAnd second line of data\n";
    const wstring existingFileName = L"existingFile.txt";
    FileTools::WriteBufferToFile(existingFileName, dummyContent);

    unsigned int beforeFileCount = GetFileNumberInCurrentFolder();
    QCOMPARE(beforeFileCount, 1u);

    JobStatus* status = RunRsnapshotJob(existingFileName);
    delete status;

    unsigned int afterFileCount = GetFileNumberInCurrentFolder();
    QCOMPARE(afterFileCount, beforeFileCount);

    const wstring fileContent = FileTools::GetTextFileContent(existingFileName);
    QCOMPARE(fileContent, dummyContent);
}

void RsnapshotJobTest::ProcessingBetweenBackups()
{
    sleep(1);
}

void RsnapshotJobTest::CheckBackedUpDataIsOk()
{
    QFETCH(QString, sourceBefore);
    FileTestUtils::CheckFoldersHaveSameContent(
             GetRsnapshotBackupFolder(1),
             QString::fromStdWString(currentTestCaseFolder) + sourceBefore);

    QFETCH(QString, sourceNow);
    FileTestUtils::CheckFoldersHaveSameContent(
             GetRsnapshotBackupFolder(0),
             QString::fromStdWString(currentTestCaseFolder) + sourceNow);

    FileTestUtils::CheckFoldersHaveSameContent(GetRsnapshotBackupFolder(0),
                                               QString::fromStdWString(currentSourceFolder));
}

AbstractBackupJob *RsnapshotJobTest::CreateNewJob()
{
    RsnapshotSmartBackupJob* job = new RsnapshotSmartBackupJob();
    job->SetTargetLocal();
    job->SetTemplateConfigurationFile(GetDataFolder() + templateConfigurationFile);
    job->SetRepository(backupRepository);
    return job;
}

JobStatus *RsnapshotJobTest::RunRsnapshotJob(const wstring &tempConfigurationFile)
{
    RsnapshotSmartBackupJob job;
    job.SetTemplateConfigurationFile(GetDataFolder() + templateConfigurationFile);
    if (tempConfigurationFile != L"")
        job.SetTemporaryFile(tempConfigurationFile);
    job.SetRepository(backupRepository);

    return job.Run();
}

QString RsnapshotJobTest::GetRsnapshotBackupFolder(const int number) const
{
    wstringstream stream;
    stream << backupRepository << "weekly." << number << "/repository";
    return QString::fromStdWString(stream.str());
}

unsigned int RsnapshotJobTest::GetFileNumberInCurrentFolder()
{
    QDir currentDir = QDir::current();
    QStringList folderList = currentDir.entryList();

    folderList.removeAll(".");
    folderList.removeAll("..");
    return folderList.size();
}

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

const string templateConfigurationFile = "rsnapshot.conf";
const string suiteFolder = "Rsnapshot/";

RsnapshotJobTest::RsnapshotJobTest(const string &dataPrefix, const string &errorPrefix)
    : AbstractFsBackupJobTest(dataPrefix + suiteFolder, errorPrefix)
{
}

RsnapshotJobTest::~RsnapshotJobTest()
{
}

void RsnapshotJobTest::testCreate_InvalidSource()
{
    AbstractBackupJob* job = CreateNewJob();
    job->AddFolder("dummySource", "dummyName");
    JobStatus* status = job->Run();
    delete job;

    QCOMPARE(status->GetCode(), JobStatus::ERROR);
    const string expectedMessage = "Tried to backup invalid folder";
    QCOMPARE(status->GetDescription(), expectedMessage);

    vector<pair<string,string> > buffers;
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
    const string dummyContent = "First line\nAnd second line of data\n";
    const string existingFileName = "existingFile.txt";
    FileTools::WriteBufferToFile(existingFileName, dummyContent);

    unsigned int beforeFileCount = GetFileNumberInCurrentFolder();
    QCOMPARE(beforeFileCount, 1u);

    JobStatus* status = RunRsnapshotJob(existingFileName);
    delete status;

    unsigned int afterFileCount = GetFileNumberInCurrentFolder();
    QCOMPARE(afterFileCount, beforeFileCount);

    const string fileContent = FileTools::GetTextFileContent(existingFileName);
    QCOMPARE(fileContent, dummyContent);
}

void RsnapshotJobTest::ProcessingBetweenBackups()
{
    sleep(1);
}

void RsnapshotJobTest::CheckBackedUpDataIsOk()
{
    QFETCH(QString, sourceBefore);
    FileTestUtils::CheckFoldersHaveSameContent(GetRsnapshotBackupFolder(1),
                                               currentTestCaseFolder + sourceBefore.toStdString());

    QFETCH(QString, sourceNow);
    FileTestUtils::CheckFoldersHaveSameContent(GetRsnapshotBackupFolder(0),
                                               currentTestCaseFolder + sourceNow.toStdString());

    FileTestUtils::CheckFoldersHaveSameContent(GetRsnapshotBackupFolder(0),
                                               currentSourceFolder);
}

AbstractBackupJob *RsnapshotJobTest::CreateNewJob()
{
    RsnapshotSmartBackupJob* job = new RsnapshotSmartBackupJob();
    job->SetTargetLocal();
    job->SetTemplateConfigurationFile(GetDataFolder() + templateConfigurationFile);
    job->SetRepository(backupRepository);
    return job;
}

JobStatus *RsnapshotJobTest::RunRsnapshotJob(const string &tempConfigurationFile)
{
    RsnapshotSmartBackupJob job;
    job.SetTemplateConfigurationFile(GetDataFolder() + templateConfigurationFile);
    if (tempConfigurationFile != "")
        job.SetTemporaryFile(tempConfigurationFile);
    job.SetRepository(backupRepository);

    return job.Run();
}

string RsnapshotJobTest::GetRsnapshotBackupFolder(const int number) const
{
    stringstream stream;
    stream << backupRepository << "weekly." << number << "/repository";
    return stream.str();
}

unsigned int RsnapshotJobTest::GetFileNumberInCurrentFolder()
{
    QDir currentDir = QDir::current();
    QStringList folderList = currentDir.entryList();

    folderList.removeAll(".");
    folderList.removeAll("..");
    return folderList.size();
}

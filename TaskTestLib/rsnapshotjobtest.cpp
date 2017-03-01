#include "rsnapshotjobtest.h"

#include <QDir>
#include <QTest>

#include <sstream>
#include <unistd.h>

#include "filetestutils.h"
#include "filetools.h"
#include "rsnapshotbackupjob.h"
#include "rsnapshotsmartcreator.h"
#include "tools.h"

using namespace std;

// TODO : remove this and find a way to use JobStatus values outside of library
#define JobStatus_NOT_EXECUTED      0
#define JobStatus_OK                1
#define JobStatus_OK_WITH_WARNINGS  2
#define JobStatus_ERROR             3

const string templateConfigurationFile = "rsnapshot.conf";
const string currentSourceFolder = "currentFolderToBackup";
const string suiteFolder = "../Rsnapshot/";

RsnapshotJobTest::RsnapshotJobTest()
{
    repository = FileTools::BuildFullPath("rsnapshotroot");
}

void RsnapshotJobTest::init()
{
    cleanup();
}

void RsnapshotJobTest::cleanup()
{
    string unusedOutput;
    Tools::RunExternalCommandToBuffer("rm -Rf *", unusedOutput, true);
}

void RsnapshotJobTest::testCreate_InvalidSource()
{
    JobStatus* status = RunRsnapshotJob();

    QCOMPARE(status->GetCode(), JobStatus_ERROR);
    const string expectedMessage = "Tried to backup invalid folder";
    QCOMPARE(status->GetDescription(), expectedMessage);

    vector<pair<string,string> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.size(), 0ul);
}

void RsnapshotJobTest::testRunBackup_data()
{
    QTest::addColumn<QString>("sourceBefore");
    QTest::addColumn<QString>("sourceNow");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QString>("report");

    QStringList testCases = FileTestUtils::GetFolderList(suiteFolder.c_str());
    for (auto it=testCases.begin(); it!=testCases.end(); ++it)
    {
        string stdString = it->toStdString();
        QTest::newRow(stdString.c_str())
                                << "sourceBefore"
                                << "sourceNow"
                                << "miniDescription.txt"
                                << "fullReport.txt";
    }
}

void RsnapshotJobTest::testRunBackup()
{
    currentTestCaseName = QTest::currentDataTag();
    currentTestCaseFolder = suiteFolder + currentTestCaseName + "/";

    QFETCH(QString, sourceBefore);
    QFETCH(QString, sourceNow);

    RunBackupOnDataFolder(currentTestCaseFolder + sourceBefore.toStdString());
    sleep(1);
    JobStatus* status = RunBackupOnDataFolder(currentTestCaseFolder + sourceNow.toStdString());
    CheckStatus(status);
    CheckFiles();
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

JobStatus* RsnapshotJobTest::RunBackupOnDataFolder(const string &folder)
{
    string unusedOutput;
    string command = string("rm -Rf ") + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    command = string("mkdir ") + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    command = string("cp -R ") + folder + "/* " + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    return RunRsnapshotJob();
}

void RsnapshotJobTest::CheckStatus(JobStatus *status)
{
    QCOMPARE(status->GetCode(), JobStatus_OK);

    QFETCH(QString, description);
    CheckTextContent(status->GetDescription(), description);

    vector<pair<string,string> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.size(), 1ul);

    QFETCH(QString, report);
    CheckTextContent(buffers.front().second, report);
}

void RsnapshotJobTest::CheckFiles()
{
    QFETCH(QString, sourceBefore);
    CheckFoldersHaveSameContent(GetRsnapshotBackupFolder(1), currentTestCaseFolder + sourceBefore.toStdString());

    QFETCH(QString, sourceNow);
    CheckFoldersHaveSameContent(GetRsnapshotBackupFolder(0), currentTestCaseFolder + sourceNow.toStdString());

    CheckFoldersHaveSameContent(GetRsnapshotBackupFolder(0), currentSourceFolder);
}

JobStatus *RsnapshotJobTest::RunRsnapshotJob(const string &tempConfigurationFile)
{
    RsnapshotSmartCreator creator(repository);
    creator.SetTemplateConfigurationFile(suiteFolder + templateConfigurationFile);
    creator.AddFolderToBackup(FileTools::BuildFullPath(currentSourceFolder), currentSourceFolder);
    if (tempConfigurationFile != "")
        creator.SetGeneratedConfigurationFile(tempConfigurationFile);

    RsnapshotBackupJob* job = creator.CreateConfiguredJob();
    JobStatus* status = job->Run();
    delete job;
    return status;
}

void RsnapshotJobTest::CheckTextContent(const string &content, const QString &referenceFile)
{
    string stdReferenceFile = currentTestCaseFolder + referenceFile.toStdString();
    string expectedContent = FileTools::GetTextFileContent(stdReferenceFile);
    bool isContentAsExpected = (content == expectedContent);
    if (!isContentAsExpected)
    {
        const string filename = currentTestCaseName + "_" + referenceFile.toStdString();
        FileTools::WriteBufferToFile(filename, content);
    }
    QCOMPARE(isContentAsExpected, true);
}

void RsnapshotJobTest::CheckFoldersHaveSameContent(const string &folder1, const string &folder2)
{
    QDir repositoryDir(folder1.c_str());
    QStringList expectedFiles = repositoryDir.entryList();
    expectedFiles.removeOne(".");
    expectedFiles.removeOne("..");

    FileTestUtils::CheckFolderContent(folder2, expectedFiles);
}

string RsnapshotJobTest::GetRsnapshotBackupFolder(const int number) const
{
    stringstream stream;
    stream << repository << "weekly." << number << "/" << currentSourceFolder;
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

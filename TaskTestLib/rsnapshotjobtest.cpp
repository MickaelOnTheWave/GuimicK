#include "rsnapshotjobtest.h"

#include <QDir>
#include <QTest>

#include <sstream>
#include <unistd.h>

#include "filetestutils.h"
#include "filetools.h"
#include "rsnapshotbackupjob.h"
#include "tools.h"

using namespace std;

// TODO : remove this and find a way to use JobStatus values outside of library
#define JobStatus_NOT_EXECUTED      0
#define JobStatus_OK                1
#define JobStatus_OK_WITH_WARNINGS  2
#define JobStatus_ERROR             3

const string templateConfigurationFile = "rsnapshot.conf";
const string defaultConfigurationFile = "backuptests.conf";
const string currentSourceFolder = "currentFolderToBackup";
const string suiteFolder = "../Rsnapshot/";

RsnapshotJobTest::RsnapshotJobTest()
{
    repository = BuildFullPathOnCurrentDir("rsnapshotroot");
}

void RsnapshotJobTest::init()
{
    cleanup();
}

void RsnapshotJobTest::cleanup()
{
    string unusedOutput;
    string removeCommand = string("rm ") + defaultConfigurationFile;
    Tools::RunExternalCommandToBuffer(removeCommand, unusedOutput, true);

    removeCommand = string("rm -Rf ") + repository;
    Tools::RunExternalCommandToBuffer(removeCommand, unusedOutput, true);

    removeCommand = string("rm -Rf ") + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(removeCommand, unusedOutput, true);
}

void RsnapshotJobTest::testCreate_InvalidSource()
{
    const QString invalidFolder = "InvalidFolder";
    CreateConfigurationFile(invalidFolder);
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

    QStringList testCases = GetTestFolders();
    for (auto it=testCases.begin(); it!=testCases.end(); ++it)
    {
        if (*it == "." || *it == "..")
            continue;

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

    CreateConfigurationFile(QString(currentSourceFolder.c_str()));

    RunBackupOnDataFolder(currentTestCaseFolder + sourceBefore.toStdString());
    sleep(1);
    JobStatus* status = RunBackupOnDataFolder(currentTestCaseFolder + sourceNow.toStdString());
    CheckStatus(status);
    CheckFiles();
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

JobStatus *RsnapshotJobTest::RunJob()
{
    QFETCH(QString, source);

    CreateConfigurationFile(source);
    JobStatus* status = RunRsnapshotJob();
    return status;
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

void RsnapshotJobTest::CreateConfigurationFile(const QString &folder)
{
    std::string contents;
    bool ok = FileTools::GetTextFileContent(suiteFolder + templateConfigurationFile, contents);
    QCOMPARE(ok, true);

    contents += string("\nsnapshot_root\t") + repository + "\n";
    contents += string("\nbackup\t") + BuildFullPathOnCurrentDir(folder.toStdString());
    contents += "\t" + folder.toStdString() + "\n";

    FileTools::WriteBufferToFile(defaultConfigurationFile, contents);
}

JobStatus *RsnapshotJobTest::RunRsnapshotJob()
{
    RsnapshotBackupJob job(repository, defaultConfigurationFile);
    return job.Run();
}

std::string RsnapshotJobTest::BuildFullPathOnCurrentDir(const std::string& name)
{
    char unusedBuffer[1024];
    char* currentPath = getcwd(unusedBuffer, 1024);
    return string(currentPath) + "/" + name + "/";
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

QStringList RsnapshotJobTest::GetTestFolders()
{
    QDir currentDir = QDir::current();
    currentDir.cd(suiteFolder.c_str());
    return currentDir.entryList(QDir::Dirs);
}

string RsnapshotJobTest::GetRsnapshotBackupFolder(const int number) const
{
    stringstream stream;
    stream << repository << "weekly." << number << "/" << currentSourceFolder;
    return stream.str();
}

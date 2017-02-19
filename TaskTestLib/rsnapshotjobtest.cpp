#include "rsnapshotjobtest.h"

#include <QDir>
#include <QTest>

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

    QTest::newRow("Example") << "sourceBefore" << "sourceNow" << "miniDescription.txt" << "fullReport.txt";
}

void RsnapshotJobTest::testRunBackup()
{
    QFETCH(QString, sourceBefore);
    QFETCH(QString, sourceNow);

    CreateConfigurationFile(QString(currentSourceFolder.c_str()));

    RunBackupOnDataFolder(sourceBefore);
    JobStatus* status = RunBackupOnDataFolder(sourceNow);
    CheckStatus(status);
    CheckFiles();
}

JobStatus* RsnapshotJobTest::RunBackupOnDataFolder(const QString &folder)
{
    string unusedOutput;
    string command = string("rm -Rf ") + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    command = string("mkdir ") + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    command = string("cp -R ") + folder.toStdString() + "/* " + currentSourceFolder;
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
    string descriptionFile = suiteFolder + description.toStdString();
    string expectedDescription = FileTools::GetTextFileContent(descriptionFile);
    QCOMPARE(status->GetDescription(), expectedDescription);

    vector<pair<string,string> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.size(), 1ul);

    QFETCH(QString, report);
    string reportFile = suiteFolder + report.toStdString();
    string expectedContent = FileTools::GetTextFileContent(reportFile);
    string content = buffers.front().second;
    QCOMPARE(content, expectedContent);
}

void RsnapshotJobTest::CheckFiles()
{
    const string repositoryBackupLocation = repository + "/weekly.0/" + currentSourceFolder;

    QFETCH(QString, sourceNow);
    QDir repositoryDir = QDir::currentPath();
    repositoryDir.cd(sourceNow);
    QStringList expectedFiles = repositoryDir.entryList();

    FileTestUtils::CheckFolderContent(currentSourceFolder, expectedFiles);
    FileTestUtils::CheckFolderContent(repositoryBackupLocation, expectedFiles);
}

void RsnapshotJobTest::CreateConfigurationFile(const QString &folder)
{
    std::string contents;
    bool ok = FileTools::GetTextFileContent(suiteFolder + templateConfigurationFile, contents);
    QCOMPARE(ok, true);

    contents += string("\nsnapshot_root\t") + repository + "\n";
    contents += string("\nbackup\t") + BuildFullPathOnCurrentDir(folder.toStdString());
    contents += "/\t" + folder.toStdString() + "\n";

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

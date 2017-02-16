#include "rsnapshotjobtest.h"

#include <QTest>

#include <unistd.h>

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
const string currentDestinationFolder = "currentDest";

RsnapshotJobTest::RsnapshotJobTest()
{
    repository = BuildFullPathOnCurrentDir("rsnapshotroot");
}

void RsnapshotJobTest::init()
{
    string unusedOutput;
    string removeCommand = string("rm ") + defaultConfigurationFile;
    Tools::RunExternalCommandToBuffer(removeCommand, unusedOutput, true);
}

void RsnapshotJobTest::cleanup()
{

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

void RsnapshotJobTest::testCreate_AllValid()
{

/*

    CreateConfigurationFile(folder);
    JobStatus* status = RunRsnapshotJob();

    QCOMPARE(status->GetCode(), JobStatus_OK);
    const string expectedMessage = "X files created.";
    QCOMPARE(status->GetDescription(), expectedMessage);

    vector<pair<string,string> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.size(), 1ul);
        QFAIL("Implementation not finished.");
*/
}

void RsnapshotJobTest::testRunBackup_data()
{
    QTest::addColumn<QString>("source");
    QTest::addColumn<QString>("destination");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QString>("report");

    QTest::newRow("ex1") << "ex1_src" << "ex1_dest" << "ex1_description.txt" << "ex1_dest.txt";
}

void RsnapshotJobTest::testRunBackup()
{
    CreateDestinationFolder();

    JobStatus* status = RunJob();
    CheckStatus(status);
    CheckFiles();

    RemoveDestinationFolder();
}

void RsnapshotJobTest::CreateDestinationFolder()
{
    QFETCH(QString, destination);

    string copyCommand = "cp -R ";
    copyCommand += destination.toStdString() + " " + currentDestinationFolder;
    string unusedOutput;
    Tools::RunExternalCommandToBuffer(copyCommand, unusedOutput);
}

void RsnapshotJobTest::RemoveDestinationFolder()
{
    string copyCommand = string("rm -Rf ") + currentDestinationFolder;
    string unusedOutput;
    Tools::RunExternalCommandToBuffer(copyCommand, unusedOutput);
}

JobStatus *RsnapshotJobTest::RunJob()
{
    QFETCH(QString, source);

    CreateConfigurationFile(source);
    JobStatus* status = RunRsnapshotJob();
    remove(defaultConfigurationFile.c_str());
    return status;
}

void RsnapshotJobTest::CheckStatus(JobStatus *status)
{
    QCOMPARE(status->GetCode(), JobStatus_OK);

    QFETCH(QString, description);
    string expectedDescription = FileTools::GetTextFileContent(description.toStdString());
    QCOMPARE(status->GetDescription(), expectedDescription);

    vector<pair<string,string> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.size(), 1ul);

    QFETCH(QString, report);
    string expectedContent = FileTools::GetTextFileContent(report.toStdString());
    string content = buffers.front().second;
    QCOMPARE(content, expectedContent);
}

void RsnapshotJobTest::CheckFiles()
{
    QFAIL("Implementation not finished.");
}

void RsnapshotJobTest::CreateConfigurationFile(const QString &folder)
{
    std::string contents;
    bool ok = FileTools::GetTextFileContent(templateConfigurationFile, contents);
    QCOMPARE(ok, true);

    contents += string("\nsnapshot_root\t") + repository + "\n";
    contents += string("\nbackup\t") + BuildFullPathOnCurrentDir(folder.toStdString());
    contents += "/\t" + currentDestinationFolder + "\n";

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

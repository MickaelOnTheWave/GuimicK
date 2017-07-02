#include "abstractfsbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "tools.h"

using namespace std;

// TODO : remove this and find a way to use JobStatus values outside of library
#define JobStatus_NOT_EXECUTED      0
#define JobStatus_OK                1
#define JobStatus_OK_WITH_WARNINGS  2
#define JobStatus_ERROR             3

AbstractFsBackupJobTest::AbstractFsBackupJobTest(const std::string &dataPrefix,
                                                 const string &errorPrefix)
    : QtTestSuite(dataPrefix, errorPrefix)
{
}

AbstractFsBackupJobTest::~AbstractFsBackupJobTest()
{
}

void AbstractFsBackupJobTest::init()
{
    cleanup();
}

void AbstractFsBackupJobTest::cleanup()
{
    string unusedOutput;
    Tools::RunExternalCommandToBuffer("rm -Rf *", unusedOutput, true);
}

void AbstractFsBackupJobTest::testRunBackup_data()
{
    LoadExternalDataSamples(false);
}

void AbstractFsBackupJobTest::testRunBackup()
{
    QFETCH(QString, sourceBefore);
    QFETCH(QString, sourceNow);
    currentTestCaseName = QTest::currentDataTag();
    currentTestCaseFolder = GetDataFolder() + currentTestCaseName + "/";
    const string beforeFolder = currentTestCaseFolder + sourceBefore.toStdString();
    const string nowFolder = currentTestCaseFolder + sourceNow.toStdString();

    RunBackupOnDataFolder(beforeFolder);

    ProcessingBetweenBackups();

    JobStatus* status = RunBackupOnDataFolder(nowFolder);

    CheckStatus(status);
    CheckBackedUpDataIsOk();
}

void AbstractFsBackupJobTest::ProcessingBetweenBackups()
{
}

void AbstractFsBackupJobTest::LoadExternalDataSamples(const bool isRemote)
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

JobStatus *AbstractFsBackupJobTest::RunBackupOnDataFolder(const string &folder)
{
    string unusedOutput;
    string command = string("rm -Rf ") + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    command = string("mkdir ") + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    command = string("cp -R ") + folder + "/* " + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    return RunBackupJob();
}

void AbstractFsBackupJobTest::CheckStatus(JobStatus *status)
{
    QVERIFY(status != nullptr);
    QCOMPARE(status->GetCode(), JobStatus_OK);

    QFETCH(QString, description);
    CheckTextContent(status->GetDescription(), description);

    vector<pair<string,string> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.size(), 1ul);

    QFETCH(QString, report);
    CheckTextContent(buffers.front().second, report);
}

void AbstractFsBackupJobTest::CheckTextContent(const string &content, const QString &referenceFile)
{
    string stdReferenceFile = currentTestCaseFolder + referenceFile.toStdString();
    string expectedContent = FileTools::GetTextFileContent(stdReferenceFile);
    bool isContentAsExpected = (content == expectedContent);
    if (!isContentAsExpected)
    {
        const string filename = currentTestCaseName + "_" + referenceFile.toStdString();
        FileTools::WriteBufferToFile(GetErrorFolder() + filename, content);
    }
    QCOMPARE(isContentAsExpected, true);
}

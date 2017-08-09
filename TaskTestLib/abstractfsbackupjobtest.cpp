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

const string repository = "repository/";
const string sshUser = "mickael";
const string sshHost = "192.168.1.101";

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

void AbstractFsBackupJobTest::CheckBackedUpDataIsOk()
{
    FileTestUtils::CheckFoldersHaveSameContent(repository, currentSourceFolder);
}

JobStatus *AbstractFsBackupJobTest::RunBackupJob()
{
    QFETCH(bool, remote);

    AbstractBackupJob* job = CreateNewJob();
    JobStatus* status = RunBackupJob(job, remote);

    delete job;
    return status;
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


void AbstractFsBackupJobTest::testCreatesOnlyOneAttachment()
{
    vector<string> expectedAttachments = {GetJobAttachmentName()};
    testCheckJobAttachments(false, expectedAttachments);
}

void AbstractFsBackupJobTest::testCreatesDebugAttachment()
{
    vector<string> expectedAttachments =
    {
        GetJobAttachmentName(),
        GetJobDebugName()
    };

    testCheckJobAttachments(true, expectedAttachments);
}

void AbstractFsBackupJobTest::testCheckJobAttachments(
        const bool debugOutput,const std::vector<string> &expectedAttachments)
{
    AbstractBackupJob* job = CreateNewJob();

    job->SetOutputDebugInformation(debugOutput);
    job->AddFolder("dummySource", "dummyDestination");
    JobStatus* status = RunBackupJob(job, false);

    JobStatus::FileBufferList fileBuffers;
    status->GetFileBuffers(fileBuffers);
    QCOMPARE(fileBuffers.size(), expectedAttachments.size());

    vector<string> attachmentNames;
    for (auto&& it : fileBuffers)
        attachmentNames.push_back(it.first);

    bool isEqual = std::equal(attachmentNames.begin(), attachmentNames.end(),
                              expectedAttachments.begin());
    QCOMPARE(isEqual, true);

    delete status;
    delete job;
}

JobStatus *AbstractFsBackupJobTest::RunBackupJob(AbstractBackupJob *job, const bool isRemote)
{
    job->InitializeFromClient(nullptr);
    job->AddFolder(FileTools::BuildFullPath(currentSourceFolder), repository);
    if (isRemote)
        job->SetTargetRemote(sshUser, sshHost);
    else
        job->SetTargetLocal();

    return job->Run();
}

string AbstractFsBackupJobTest::GetJobAttachmentName()
{
    AbstractBackupJob* dummyJob = CreateNewJob();
    string name = dummyJob->GetAttachmentName();
    delete dummyJob;

    return name;
}

// TODO : remove duplication with debug manager
string AbstractFsBackupJobTest::GetJobDebugName()
{
    AbstractBackupJob* dummyJob = CreateNewJob();
    string name = dummyJob->GetName();
    delete dummyJob;

    return name + " - Debug.txt";
}

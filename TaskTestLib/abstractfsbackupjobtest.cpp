#include "abstractfsbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "tools.h"

using namespace std;

static const string sshUser = "mickael";
static const string sshHost = "192.168.1.101";

AbstractFsBackupJobTest::AbstractFsBackupJobTest(const std::string &dataPrefix,
                                                 const string &errorPrefix)
    : AbstractBackupJobTest(dataPrefix, errorPrefix)
{
}

void AbstractFsBackupJobTest::testRunBackup_data()
{
    LoadExternalDataSamples(false);
}

void AbstractFsBackupJobTest::testRunBackup()
{
    QFETCH(QString, sourceBefore);
    QFETCH(QString, sourceNow);
    QFETCH(bool, remote);
    currentTestCaseName = QTest::currentDataTag();
    currentTestCaseFolder = GetDataFolder() + currentTestCaseName + "/";
    const string beforeFolder = currentTestCaseFolder + sourceBefore.toStdString();
    const string nowFolder = currentTestCaseFolder + sourceNow.toStdString();

    JobStatus* status = RunBackups(beforeFolder, nowFolder, remote);

    CheckStatus(status);
    CheckBackedUpDataIsOk();
    delete status;
}

void AbstractFsBackupJobTest::ProcessingBetweenBackups()
{
}

void AbstractFsBackupJobTest::CheckBackedUpDataIsOk()
{
   FileTestUtils::CheckFoldersHaveSameContent(backupRepository, currentSourceFolder);
}

JobStatus *AbstractFsBackupJobTest::RunBackupJob(const bool isRemote,
                                                 const bool useDebug)
{
    AbstractBackupJob* job = CreateNewJob();
    JobStatus* status = RunBackupJob(job, isRemote, useDebug);

    delete job;
    return status;
}

JobStatus* AbstractFsBackupJobTest::RunBackups(const string &folderBefore,
                                               const string &folderNow,
                                               const bool isRemote,
                                               const bool useDebug)
{
    JobStatus* unusedStatus = RunBackupOnDataFolder(folderBefore, isRemote, useDebug);
    delete unusedStatus;

    ProcessingBetweenBackups();

    return RunBackupOnDataFolder(folderNow, isRemote, useDebug);
}

JobStatus *AbstractFsBackupJobTest::RunBackupOnDataFolder(const string &folder,
                                                          const bool isRemote,
                                                          const bool useDebug)
{
    string unusedOutput;
    string command = string("rm -Rf ") + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    command = string("mkdir ") + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    command = string("cp -R ") + folder + "/* " + currentSourceFolder;
    Tools::RunExternalCommandToBuffer(command, unusedOutput, true);

    return RunBackupJob(isRemote, useDebug);
}

void AbstractFsBackupJobTest::CheckStatus(JobStatus *status)
{
    QVERIFY(status != nullptr);
    QCOMPARE(status->GetCode(), JobStatus::OK);

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
    JobStatus* status = RunDummyBackup(debugOutput);
    CheckStatusAttachments(status, expectedAttachments);
    delete status;
}

JobStatus *AbstractFsBackupJobTest::RunBackupJob(AbstractBackupJob *job,
                                                 const bool isRemote,
                                                 const bool useDebug)
{
    job->InitializeFromClient(nullptr);
    job->AddFolder(currentSourceFolder, backupRepository);

    if (isRemote)
        job->SetTargetRemote(sshUser, sshHost);
    else
        job->SetTargetLocal();

    job->SetOutputDebugInformation(useDebug ? DebugOutput::ALWAYS : DebugOutput::NEVER);

    return job->Run();
}

JobStatus *AbstractFsBackupJobTest::RunDummyBackup(const bool debugOutput)
{
    const string dummyBefore = "dummyBefore";
    const string dummyNow = "dummyNow";

    FileTools::CreateFolder(dummyBefore);
    FileTools::CreateFolder(dummyNow);

    return RunBackups(dummyBefore, dummyNow, false, debugOutput);
}

void AbstractFsBackupJobTest::CheckStatusAttachments(JobStatus *status,
                                                     const std::vector<string> &expectedAttachments)
{
    JobStatus::FileBufferList fileBuffers;
    status->GetFileBuffers(fileBuffers);
    QCOMPARE(fileBuffers.size(), expectedAttachments.size());

    vector<string> attachmentNames;
    for (auto&& it : fileBuffers)
        attachmentNames.push_back(it.first);

    bool isEqual = std::equal(attachmentNames.begin(), attachmentNames.end(),
                              expectedAttachments.begin());
    QCOMPARE(isEqual, true);
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

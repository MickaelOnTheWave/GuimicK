#include "abstractfsbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"
#include "stringtools.h"
#include "tools.h"

using namespace std;

static const wstring sshUser = L"mickael";
static const wstring sshHost = L"192.168.1.101";

AbstractFsBackupJobTest::AbstractFsBackupJobTest(const wstring &dataPrefix,
                                                 const wstring &errorPrefix)
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
    currentTestCaseName = StringTools::Utf8ToUnicode(QTest::currentDataTag());
    currentTestCaseFolder = GetDataFolder() + currentTestCaseName + L"/";
    const wstring beforeFolder = currentTestCaseFolder + sourceBefore.toStdWString();
    const wstring nowFolder = currentTestCaseFolder + sourceNow.toStdWString();

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
   FileTestUtils::CheckFoldersHaveSameContent(
            QString::fromStdWString(backupRepository),
            QString::fromStdWString(currentSourceFolder));
}

JobStatus *AbstractFsBackupJobTest::RunBackupJob(const bool isRemote,
                                                 const bool useDebug)
{
    AbstractBackupJob* job = CreateNewJob();
    JobStatus* status = RunBackupJob(job, isRemote, useDebug);

    delete job;
    return status;
}

JobStatus* AbstractFsBackupJobTest::RunBackups(const wstring &folderBefore,
                                               const wstring &folderNow,
                                               const bool isRemote,
                                               const bool useDebug)
{
    JobStatus* unusedStatus = RunBackupOnDataFolder(folderBefore, isRemote, useDebug);
    delete unusedStatus;

    ProcessingBetweenBackups();

    return RunBackupOnDataFolder(folderNow, isRemote, useDebug);
}

JobStatus *AbstractFsBackupJobTest::RunBackupOnDataFolder(const wstring &folder,
                                                          const bool isRemote,
                                                          const bool useDebug)
{
   currentSourceFolder = folder;
   return RunBackupJob(isRemote, useDebug);
}

void AbstractFsBackupJobTest::CheckStatus(JobStatus *status)
{
    QVERIFY(status != nullptr);
    QCOMPARE(status->GetCode(), JobStatus::Ok);

    QFETCH(QString, description);
    CheckTextContent(status->GetDescription(), description);

    vector<pair<wstring,wstring> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.size(), 1ul);

    QFETCH(QString, report);
    CheckTextContent(buffers.front().second, report);
}

void AbstractFsBackupJobTest::CheckTextContent(const wstring &content, const QString &referenceFile)
{
    wstring stdReferenceFile = currentTestCaseFolder + referenceFile.toStdWString();
    wstring expectedContent = FileTools::GetTextFileContent(stdReferenceFile);
    bool isContentAsExpected = (content == expectedContent);
    if (!isContentAsExpected)
    {
        const wstring filename = currentTestCaseName + L"_" + referenceFile.toStdWString();
        FileTools::WriteBufferToFile(GetErrorFolder() + filename, content);
    }
    QCOMPARE(isContentAsExpected, true);
}


void AbstractFsBackupJobTest::testCreatesOnlyOneAttachment()
{
    vector<wstring> expectedAttachments = {GetJobAttachmentName()};
    testCheckJobAttachments(false, expectedAttachments);
}

void AbstractFsBackupJobTest::testCreatesDebugAttachment()
{
    vector<wstring> expectedAttachments =
    {
        GetJobAttachmentName(),
        GetJobDebugName()
    };

    testCheckJobAttachments(true, expectedAttachments);
}

void AbstractFsBackupJobTest::testCheckJobAttachments(
        const bool debugOutput,const vector<wstring> &expectedAttachments)
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
    const wstring dummyBefore = L"dummyBefore";
    const wstring dummyNow = L"dummyNow";

    FileTools::CreateFolder(dummyBefore);
    FileTools::CreateFolder(dummyNow);

    return RunBackups(dummyBefore, dummyNow, false, debugOutput);
}

void AbstractFsBackupJobTest::CheckStatusAttachments(JobStatus *status,
                                                     const vector<wstring> &expectedAttachments)
{
    JobStatus::FileBufferList fileBuffers;
    status->GetFileBuffers(fileBuffers);
    QCOMPARE(fileBuffers.size(), expectedAttachments.size());

    vector<wstring> attachmentNames;
    for (auto&& it : fileBuffers)
        attachmentNames.push_back(it.first);

    bool isEqual = std::equal(attachmentNames.begin(), attachmentNames.end(),
                              expectedAttachments.begin());
    QCOMPARE(isEqual, true);
}

wstring AbstractFsBackupJobTest::GetJobAttachmentName()
{
    AbstractBackupJob* dummyJob = CreateNewJob();
    const wstring name = dummyJob->GetAttachmentName();
    delete dummyJob;

    return name;
}

// TODO : remove duplication with debug manager
wstring AbstractFsBackupJobTest::GetJobDebugName()
{
    AbstractBackupJob* dummyJob = CreateNewJob();
    const wstring name = dummyJob->GetName();
    delete dummyJob;

    return name + L" - Debug.txt";
}

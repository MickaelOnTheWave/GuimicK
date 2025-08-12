#include "sshconsolejobtest.h"

#include <QTest>
#include <sstream>

#include "filetools.h"
#include "pathtools.h"
#include "stringtools.h"
#include "testutils.h"
#include "userconsolejob.h"

using namespace std;

SshConsoleJobTest::SshConsoleJobTest()
{
}

void SshConsoleJobTest::testRun_NoTargetError()
{
    job = new SshConsoleJob(L"", L"ls");

    RunAndCheckNoAttachments(JobStatus::Error, SshConsoleJob::NoTargetError);
}

void SshConsoleJobTest::testRun_InvalidTargetError()
{
    job = new SshConsoleJob(L"", L"ls");
    GetJob()->SetTarget(L"invalidUser", L"invalidHost");

    RunAndCheckNoAttachments(JobStatus::Error, SshConsoleJob::InvalidTargetError);
}

void SshConsoleJobTest::testRun_CheckResultingCommand()
{
    const wstring contentToWrite = L"my super content\nOn multiple lines.";
    const wstring filename = L"testing.txt";
    FileTools::WriteBufferToFile(filename, contentToWrite);

    CreateRemoteCatJob(filename);

    RunAndCheckOneAttachment(JobStatus::Ok, L"", contentToWrite);
}

void SshConsoleJobTest::testRun_NoTtyError()
{
    job = new SshConsoleJob(L"", L"sudo ls /");
    GetJob()->SetTarget(L"mickael", L"Desktop");

    RunAndCheckNoAttachments(JobStatus::Error, SshConsoleJob::NoTerminalForPasswordError);
}

void SshConsoleJobTest::testRun_AttachRemoteAttachments_Ok()
{
   const wstring remoteFileName = L"Documents/Internet LinhaLivre.txt";
   job = new SshConsoleJob(new UserConsoleJob(L"", L"echo", L"blabla"));
   GetJob()->SetTarget(L"mickael", L"Desktop");
   GetJob()->AddUserAttachment(remoteFileName);

   RunAndCheck(JobStatus::Ok, L"");

   vector<pair<wstring, wstring> > fileBuffers;
   status->GetFileBuffers(fileBuffers);
   QCOMPARE(fileBuffers.size(), 0ul);

   vector<wstring> externalFiles;
   status->GetExternalFilenames(externalFiles);
   QCOMPARE(externalFiles.size(), 1ul);

   const wstring referenceFile = wstring(L"~/") + StringTools::EscapedSpaces(remoteFileName);
   const wstring referenceContent = FileTools::GetTextFileContent(referenceFile);
   const wstring actualContent = FileTools::GetTextFileContent(externalFiles.front());

   TestUtils::Compare(actualContent, referenceContent);
}

AbstractConsoleJob *SshConsoleJobTest::CreateDefaultJob(const wstring &command,
                                                        const wstring& params)
{
    SshConsoleJob* localJob = new SshConsoleJob(L"", command + L" " + params);
    localJob->SetTarget(L"mickael", L"Desktop");
    return localJob;
}

SshConsoleJob *SshConsoleJobTest::GetJob()
{
    return dynamic_cast<SshConsoleJob*>(job);
}

void SshConsoleJobTest::CreateRemoteCatJob(const wstring& fileToTest)
{
    const wstring catParams = PathTools::GetCurrentFullPath() + L"/" + fileToTest;
    UserConsoleJob* embeddedJob = new UserConsoleJob(L"", L"cat", catParams);
    embeddedJob->SetAttachOutput(true);
    job = new SshConsoleJob(embeddedJob);
    GetJob()->SetTarget(L"mickael", L"Desktop");
}

void SshConsoleJobTest::TestCommandWithAppendedParameter()
{
    TestCommandWithParameter(JobStatus::Ok, L"", L"my super content", true);
}

void SshConsoleJobTest::TestCommandWithSeparatedParameter()
{
    TestCommandWithAppendedParameter();
}

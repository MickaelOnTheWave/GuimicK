#include "sshconsolejobtest.h"

#include <QTest>
#include <sstream>

#include "filetools.h"
#include "userconsolejob.h"
#include "tools.h"

using namespace std;

SshConsoleJobTest::SshConsoleJobTest()
{
}

void SshConsoleJobTest::testRun_NoTargetError()
{
    job = new SshConsoleJob("", "ls");

    RunAndCheckNoAttachments(JobStatus::Error, SshConsoleJob::NoTargetError);
}

void SshConsoleJobTest::testRun_InvalidTargetError()
{
    job = new SshConsoleJob("", "ls");
    GetJob()->SetTarget("invalidUser", "invalidHost");

    RunAndCheckNoAttachments(JobStatus::Error, SshConsoleJob::InvalidTargetError);
}

void SshConsoleJobTest::testRun_CheckResultingCommand()
{
    const string contentToWrite = "my super content\nOn multiple lines.";
    const string filename = "testing.txt";
    FileTools::WriteBufferToFile(filename, contentToWrite);

    CreateRemoteCatJob(filename);

    RunAndCheckOneAttachment(JobStatus::Ok, "", contentToWrite);
}

void SshConsoleJobTest::testRun_NoTtyError()
{
    job = new SshConsoleJob("", "sudo ls /");
    GetJob()->SetTarget("mickael", "Desktop");

    RunAndCheckNoAttachments(JobStatus::Error, SshConsoleJob::NoTerminalForPasswordError);
}

void SshConsoleJobTest::testRun_AttachRemoteAttachments_Ok()
{
   const string remoteFileName = "Documents/Internet LinhaLivre.txt";
   job = new SshConsoleJob(new UserConsoleJob("", "echo", "blabla"));
   GetJob()->SetTarget("mickael", "Desktop");
   GetJob()->AddUserAttachment(remoteFileName);

   RunAndCheck(JobStatus::Ok, "");

   vector<pair<string, string> > fileBuffers;
   status->GetFileBuffers(fileBuffers);
   QCOMPARE(fileBuffers.size(), 0ul);

   vector<string> externalFiles;
   status->GetExternalFilenames(externalFiles);
   QCOMPARE(externalFiles.size(), 1ul);

   const string referenceFile = string("~/") + Tools::EscapedSpaces(remoteFileName);
   const string referenceContent = FileTools::GetTextFileContent(referenceFile);
   const string actualContent = FileTools::GetTextFileContent(externalFiles.front());

   QCOMPARE(actualContent.c_str(), referenceContent.c_str());

}

AbstractConsoleJob *SshConsoleJobTest::CreateDefaultJob(const std::string &command,
                                                        const string& params)
{
    SshConsoleJob* localJob = new SshConsoleJob("", command + " " + params);
    localJob->SetTarget("mickael", "Desktop");
    return localJob;
}

SshConsoleJob *SshConsoleJobTest::GetJob()
{
    return dynamic_cast<SshConsoleJob*>(job);
}

void SshConsoleJobTest::CreateRemoteCatJob(const string &fileToTest)
{
    const string catParams = FileTools::GetCurrentFullPath() + "/" + fileToTest;
    UserConsoleJob* embeddedJob = new UserConsoleJob("", "cat", catParams);
    embeddedJob->SetAttachOutput(true);
    job = new SshConsoleJob(embeddedJob);
    GetJob()->SetTarget("mickael", "Desktop");
}

void SshConsoleJobTest::TestCommandWithAppendedParameter()
{
    TestCommandWithParameter(JobStatus::Ok, "", "my super content", true);
}

void SshConsoleJobTest::TestCommandWithSeparatedParameter()
{
    TestCommandWithAppendedParameter();
}

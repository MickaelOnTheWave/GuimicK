#include "sshconsolejobtest.h"

#include <QTest>
#include <sstream>

#include "filetools.h"
#include "userconsolejob.h"

using namespace std;

SshConsoleJobTest::SshConsoleJobTest()
{
}

void SshConsoleJobTest::testRun_NoTargetError()
{
    job = new SshConsoleJob("", "ls");

    RunAndCheckNoAttachments(JobStatus::ERROR, "No target specified");
}

void SshConsoleJobTest::testRun_InvalidTargetError()
{
    job = new SshConsoleJob("", "ls");
    GetJob()->SetTarget("invalidUser", "invalidHost");

    RunAndCheckNoAttachments(JobStatus::ERROR, "Invalid target specified");
}

void SshConsoleJobTest::testRun_CheckResultingCommand()
{
    const string contentToWrite = "my super content\nOn multiple lines.";
    const string filename = "testing.txt";
    FileTools::WriteBufferToFile(filename, contentToWrite);

    CreateRemoteCatJob(filename);

    RunAndCheckOneAttachment(JobStatus::OK, "", contentToWrite);
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
    job = new SshConsoleJob("", embeddedJob);
    GetJob()->SetTarget("mickael", "Desktop");
}

void SshConsoleJobTest::TestCommandWithAppendedParameter()
{
    TestCommandWithParameter(JobStatus::OK, "", "my super content", true);
}

void SshConsoleJobTest::TestCommandWithSeparatedParameter()
{
    TestCommandWithAppendedParameter();
}

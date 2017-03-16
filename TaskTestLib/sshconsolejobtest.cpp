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

    RunAndCheckNoAttachments(JobStatus_ERROR, "No target specified");
}

void SshConsoleJobTest::testRun_InvalidTargetError()
{
    job = new SshConsoleJob("", "ls");
    GetJob()->SetTarget("invalidUser", "invalidHost");

    RunAndCheckNoAttachments(JobStatus_ERROR, "Invalid target specified");
}

void SshConsoleJobTest::testRun_CheckResultingCommand()
{
    const string contentToWrite = "my super content\nOn multiple lines.";
    const string filename = "testing.txt";
    FileTools::WriteBufferToFile(filename, contentToWrite);

    CreateRemoteCatJob(filename);

    RunAndCheckOneAttachment(JobStatus_OK, "", contentToWrite);
}

AbstractConsoleJob *SshConsoleJobTest::CreateDefaultJob(const std::string &command)
{
    SshConsoleJob* localJob = new SshConsoleJob("", command);
    localJob->SetTarget("mickael", "Desktop");
    return localJob;
}

SshConsoleJob *SshConsoleJobTest::GetJob()
{
    return dynamic_cast<SshConsoleJob*>(job);
}

void SshConsoleJobTest::CreateRemoteCatJob(const string &fileToTest)
{
    const string fullCatCommand = string("cat ") + FileTools::GetCurrentFullPath() + "/" + fileToTest;
    UserConsoleJob* embeddedJob = new UserConsoleJob("", fullCatCommand);
    embeddedJob->SetAttachOutput(true);
    job = new SshConsoleJob("", embeddedJob);
    GetJob()->SetTarget("mickael", "Desktop");
}

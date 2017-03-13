#include "userconsolejobtest.h"

#include "filetools.h"
#include <QTest>
#include <sstream>

using namespace std;

UserConsoleJobTest::UserConsoleJobTest()
{
}

void UserConsoleJobTest::testRun_InvalidCommand()
{
    const string expectedDescription = GetExpectedErrorDescription(0, 127);
    job = CreateDefaultJob("nonexistentcommand");

    RunAndCheckNoAttachments(JobStatus_ERROR, expectedDescription);

    GetJob()->SetAttachOutput(true);

    RunAndCheckOneAttachment(JobStatus_ERROR,
                             expectedDescription,
                             "sh: 1: nonexistentcommand: not found");
}

void UserConsoleJobTest::testRun_CheckReturnCode()
{
    job = ConsoleJobTest::CreateDefaultJob();

    job->SetExpectedReturnCode(0);
    RunAndCheckNoAttachments(JobStatus_OK, "");

    job->SetExpectedReturnCode(1);
    RunAndCheckNoAttachments(JobStatus_ERROR, GetExpectedErrorDescription(1, 0));
}

void UserConsoleJobTest::testRun_CheckOutput()
{
    FileTools::WriteBufferToFile("testFile", "test content");
    job = ConsoleJobTest::CreateDefaultJob();

    GetJob()->EnableSuccessOnOutput("testFile");
    RunAndCheckNoAttachments(JobStatus_OK, "");

    GetJob()->EnableSuccessOnOutput("Output is not that");
    RunAndCheckNoAttachments(JobStatus_ERROR, "Received <testFile> - expected <Output is not that>\n");
}

void UserConsoleJobTest::testRun_CheckAttachment()
{
    FileTools::WriteBufferToFile("testFile", "test content");
    job = ConsoleJobTest::CreateDefaultJob();

    RunAndCheckNoAttachments(JobStatus_OK, "");

    GetJob()->SetAttachOutput(true);
    RunAndCheckOneAttachment(JobStatus_OK, "", "testFile");
}

void UserConsoleJobTest::testRun_OutputToFile()
{
    FileTools::WriteBufferToFile("testFile", "test content");
    job = CreateDefaultJob("ls testFile");
    GetJob()->SetOutputTofile("outputFile");

    RunAndCheck(JobStatus_OK, "");
    CheckAttachmentCount(1, 0);

    std::string content = FileTools::GetTextFileContent("outputFile");
    QCOMPARE(content.c_str(), "testFile\n");
}

string UserConsoleJobTest::GetExpectedErrorDescription(const int expectedCode,
                                                       const int receivedCode)
{
    stringstream stream;
    stream << "Return value : " << receivedCode << " - expected : " << expectedCode << endl;
    return stream.str();
}

ConsoleJob *UserConsoleJobTest::CreateDefaultJob(const std::string &command)
{
    return new UserConsoleJob("dummyName", command);
}

UserConsoleJob *UserConsoleJobTest::GetJob()
{
    return dynamic_cast<UserConsoleJob*>(job);
}

void UserConsoleJobTest::RunAndCheckOneAttachment(const int expectedCode,
                                              const string &expectedDescription,
                                              const string &expectedAttachmentContent)
{
    RunAndCheck(expectedCode, expectedDescription);
    CheckAttachmentCount(0, 1);

    vector<pair<string,string> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.front().second.c_str(), expectedAttachmentContent.c_str());
}

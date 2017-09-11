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
    job = CreateDefaultJob("nonexistentcommand");

    RunAndCheckNoAttachments(JobStatus::ERROR, ConsoleJob::NotAvailableError);

    GetJob()->SetAttachOutput(true);

    // Even though job is marked to attach output, as command is invalid,
    // nothing is attached.
    RunAndCheckNoAttachments(JobStatus::ERROR, ConsoleJob::NotAvailableError);
}

void UserConsoleJobTest::testRun_CheckReturnCode()
{
    job = ConsoleJobTest::CreateDefaultJob();

    job->SetExpectedReturnCode(0);
    RunAndCheckNoAttachments(JobStatus::OK, "");

    job->SetExpectedReturnCode(1);
    RunAndCheckNoAttachments(JobStatus::ERROR, GetExpectedErrorDescription(1, 0));
}

void UserConsoleJobTest::testRun_CheckOutput()
{
    FileTools::WriteBufferToFile("testFile", "test content");
    job = ConsoleJobTest::CreateDefaultJob();

    GetJob()->SetExpectedOutput("testFile");
    RunAndCheckNoAttachments(JobStatus::OK, "");

    GetJob()->SetExpectedOutput("Output is not that");
    RunAndCheckNoAttachments(JobStatus::ERROR, GetExpectedErrorDescription("Output is not that", "testFile"));
}

void UserConsoleJobTest::testRun_CheckAttachment()
{
    FileTools::WriteBufferToFile("testFile", "test content");
    job = ConsoleJobTest::CreateDefaultJob();

    RunAndCheckNoAttachments(JobStatus::OK, "");

    GetJob()->SetAttachOutput(true);
    RunAndCheckOneAttachment(JobStatus::OK, "", "testFile");
}

void UserConsoleJobTest::testRun_OutputToFile_ReturnCode()
{
    FileTools::WriteBufferToFile("testFile", "test content");
    job = CreateDefaultJob("ls",  "testFile");
    GetJob()->SetOutputTofile("outputFile");

    RunAndCheck(JobStatus::OK, "");
    CheckAttachmentCount(1, 0);

    string content = FileTools::GetTextFileContent("outputFile");
    QCOMPARE(content.c_str(), "testFile\n");
}

void UserConsoleJobTest::testRun_OutputToFile_OutputDoesNotWork()
{
    const string testFileName = "testFile";
    const string testFileContent = "test content";
    const string outputFileName = "output";

    FileTools::WriteBufferToFile(testFileName, testFileContent);
    job = CreateDefaultJob("cat", testFileName);
    GetJob()->SetOutputTofile(outputFileName);

    GetJob()->SetExpectedOutput(testFileContent);
    RunAndCheck(JobStatus::ERROR, GetExpectedErrorDescription(testFileContent, ""));
    CheckAttachmentCount(1, 0);

    const string wrongContent = "wrong content";
    GetJob()->SetExpectedOutput(wrongContent);
    RunAndCheck(JobStatus::ERROR, GetExpectedErrorDescription(wrongContent, ""));
    CheckAttachmentCount(1, 0);
}

void UserConsoleJobTest::testConfiguration_CheckConditions()
{
    job = ConsoleJobTest::CreateDefaultJob();
    CheckJobConditions(0, "");

    GetJob()->SetExpectedReturnCode(2);
    CheckJobConditions(2, "");

    GetJob()->SetExpectedOutput("blabla");
    CheckJobConditions(-1, "blabla");

    GetJob()->SetExpectedReturnCode(3);
    CheckJobConditions(3, "");
}

string UserConsoleJobTest::GetExpectedErrorDescription(const int expectedCode,
                                                       const int receivedCode)
{
    stringstream stream;
    stream << "Return value : " << receivedCode << " - expected : " << expectedCode << endl;
    return stream.str();
}

string UserConsoleJobTest::GetExpectedErrorDescription(const string &expected, const string &received)
{
    stringstream stream;
    stream << "Received <" << received << "> - expected <" << expected << ">" << endl;
    return stream.str();
}

ConsoleJob *UserConsoleJobTest::CreateDefaultJob(const std::string &command, const string &params)
{
    return new UserConsoleJob("dummyName", command, params);
}

UserConsoleJob *UserConsoleJobTest::GetJob()
{
    return dynamic_cast<UserConsoleJob*>(job);
}

void UserConsoleJobTest::CheckJobConditions(const int expectedCode,
                                            const string &expectedOutput)
{
    QCOMPARE(GetJob()->GetExpectedReturnCode(), expectedCode);
    QCOMPARE(GetJob()->GetExpectedOutput().c_str(), expectedOutput.c_str());
}

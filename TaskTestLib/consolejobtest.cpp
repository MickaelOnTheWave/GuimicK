#include "consolejobtest.h"

#include <QTest>

#include "filetools.h"
#include "tools.h"

// TODO : remove this and find a way to use JobStatus values outside of library
#define JobStatus_NOT_EXECUTED      0
#define JobStatus_OK                1
#define JobStatus_OK_WITH_WARNINGS  2
#define JobStatus_ERROR             3

using namespace std;

ConsoleJobTest::ConsoleJobTest()
{
}

void ConsoleJobTest::init()
{
    string unused;
    Tools::RunExternalCommandToBuffer("rm -Rf *", unused);
}

void ConsoleJobTest::cleanup()
{
    delete job;
    delete status;
}

void ConsoleJobTest::testRun_InvalidCommand()
{
    job = new ConsoleJob("", "nonexistentcommand");

    status = job->Run();
    QCOMPARE(status->GetCode(), JobStatus_ERROR);
}

void ConsoleJobTest::testRun_CheckReturnCode()
{
    job = new ConsoleJob("", "ls");

    job->EnableSuccessOnReturnCode(0);
    RunAndCheckNoAttachments(JobStatus_OK, "");

    job->EnableSuccessOnReturnCode(1);
    RunAndCheckNoAttachments(JobStatus_ERROR, "Return value : 0 - expected : 1\n");
}

void ConsoleJobTest::testRun_CheckOutput()
{
    FileTools::WriteBufferToFile("testFile", "test content");
    job = new ConsoleJob("", "ls");

    job->EnableSuccessOnOutput("testFile");
    RunAndCheckNoAttachments(JobStatus_OK, "testFile");

    job->EnableSuccessOnOutput("Output is not that");
    RunAndCheckNoAttachments(JobStatus_ERROR, "Received <testFile> - expected <Output is not that>\n");
}

void ConsoleJobTest::testRun_CheckAttachment()
{
    FileTools::WriteBufferToFile("testFile", "test content");
    job = new ConsoleJob("", "ls");

    RunAndCheckNoAttachments(JobStatus_OK, "");

    job->AttachOutputToStatus();
    RunAndCheckWithAttachment(JobStatus_OK, "");
}

void ConsoleJobTest::testRun_OutputToFile()
{
    FileTools::WriteBufferToFile("testFile", "test content");
    job = new ConsoleJob("", "ls testFile");
    job->SetOutputTofile("outputFile");

    RunAndCheck(JobStatus_OK, "");
    CheckAttachmentCount(1, 0);

    string content = FileTools::GetTextFileContent("outputFile");
    QCOMPARE(content.c_str(), "testFile\n");
}

void ConsoleJobTest::RunAndCheckNoAttachments(const int expectedCode,
                                              const std::string &expectedDescription)
{
    RunAndCheck(expectedCode, expectedDescription);
    CheckAttachmentCount(0, 0);
}

void ConsoleJobTest::RunAndCheckWithAttachment(const int expectedCode,
                                               const std::string& expectedDescription)
{
    RunAndCheck(expectedCode, expectedDescription);
    CheckAttachmentCount(0, 1);

    vector<pair<string,string> > buffers;
    status->GetFileBuffers(buffers);
    QCOMPARE(buffers.front().second.c_str(), "testFile");
}

void ConsoleJobTest::RunAndCheck(const int expectedCode,
                                 const string &expectedDescription)
{
    status = job->Run();
    QCOMPARE(status->GetCode(), expectedCode);
    QCOMPARE(status->GetDescription().c_str(), expectedDescription.c_str());
}

void ConsoleJobTest::CheckAttachmentCount(const unsigned long fileCount,
                                          const unsigned long bufferCount)
{
    vector<string> filenames;
    vector<pair<string, string> > filebuffers;
    status->GetExternalFilenames(filenames);
    status->GetFileBuffers(filebuffers);
    QCOMPARE(filenames.size(), fileCount);
    QCOMPARE(filebuffers.size(), bufferCount);
}

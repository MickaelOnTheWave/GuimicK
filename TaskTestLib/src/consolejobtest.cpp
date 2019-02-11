#include "consolejobtest.h"

#include <QTest>

#include "filetools.h"
#include "tools.h"
#include "userconsolejob.h"

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
    status = nullptr;
}

void ConsoleJobTest::testRunOk()
{
    job = CreateDefaultJob();
    RunAndCheckNoAttachments(JobStatus::Ok,
                             GetExpectedOkDescription());

    job->SetExpectedReturnCode(1);
    RunAndCheckNoAttachments(JobStatus::Error,
                             GetExpectedErrorDescription(1, 0));
}

void ConsoleJobTest::testRunError()
{
    job = CreateDefaultJob("ls", "nonexistingfolder");
    RunAndCheckNoAttachments(JobStatus::Error,
                             GetExpectedErrorDescription(0, 2));
}

void ConsoleJobTest::testCommandWithAppendedParameter()
{
    TestCommandWithAppendedParameter();
}

void ConsoleJobTest::testCommandWithSeparatedParameter()
{
    TestCommandWithSeparatedParameter();
}

void ConsoleJobTest::TestCommandWithParameter(const int expectedCode,
                                              const string &expectedMessage,
                                              const string &expectedOutput,
                                              const bool appendCommand)
{
    const string fileContent = "my super content";
    const string filename = "myfile.txt";
    FileTools::WriteBufferToFile(filename, fileContent);

    const string fullFilename = FileTools::GetCurrentFullPath() + "/" + filename;

    const string catCommand = (appendCommand) ? string("cat ") + fullFilename : string("cat");
    const string catParam = (appendCommand) ? string("") : fullFilename;

    job = CreateDefaultJob(catCommand, catParam);
    RunAndCheckNoAttachments(expectedCode, expectedMessage);

    QCOMPARE(job->GetCommandOutput().c_str(), expectedOutput.c_str());
}

string ConsoleJobTest::GetExpectedOkDescription()
{
    return string("");
}

string ConsoleJobTest::GetExpectedErrorDescription(const int, const int)
{
    return string("");
}

AbstractConsoleJob *ConsoleJobTest::CreateDefaultJob(void)
{
    return CreateDefaultJob("ls");
}

AbstractConsoleJob *ConsoleJobTest::CreateDefaultJob(const string &command, const string &params)
{
    return new ConsoleJob(command, params);
}

void ConsoleJobTest::RunAndCheckNoAttachments(const int expectedCode,
                                              const std::string &expectedDescription)
{
    RunAndCheck(expectedCode, expectedDescription);
    CheckAttachmentCount(0, 0);
}

void ConsoleJobTest::RunAndCheckOneAttachment(const int expectedCode,
                                              const string &expectedDescription,
                                              const string &expectedAttachmentContent)
{
    RunAndCheck(expectedCode, expectedDescription);
    CheckAttachmentCount(0, 1);

    vector<pair<string,string> > buffers;
    status->GetFileBuffers(buffers);
    if (buffers.size() == 0)
        QFAIL("No attachment to check");
    else
        QCOMPARE(buffers.front().second.c_str(), expectedAttachmentContent.c_str());
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

void ConsoleJobTest::TestCommandWithAppendedParameter()
{
    TestCommandWithParameter(JobStatus::Ok, "", "my super content", true);
}

void ConsoleJobTest::TestCommandWithSeparatedParameter()
{
    TestCommandWithParameter(JobStatus::Ok, "", "my super content", false);
}

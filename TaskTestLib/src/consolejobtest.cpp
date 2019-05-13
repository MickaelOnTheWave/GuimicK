#include "consolejobtest.h"

#include <QTest>

#include "filetools.h"
#include "pathtools.h"
#include "tools.h"
#include "userconsolejob.h"

using namespace std;

ConsoleJobTest::ConsoleJobTest()
{
}

void ConsoleJobTest::init()
{
    wstring unused;
    Tools::RunExternalCommandToBuffer(L"rm -Rf *", unused);
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
    job = CreateDefaultJob(L"ls", L"nonexistingfolder");
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
                                              const wstring &expectedMessage,
                                              const wstring &expectedOutput,
                                              const bool appendCommand)
{
    const wstring fileContent = L"my super content";
    const wstring filename = L"myfile.txt";
    FileTools::WriteBufferToFile(filename, fileContent);

    const wstring fullFilename = PathTools::GetCurrentFullPath() + L"/" + filename;

    const wstring catCommand = (appendCommand) ? wstring(L"cat ") + fullFilename : wstring(L"cat");
    const wstring catParam = (appendCommand) ? wstring(L"") : fullFilename;

    job = CreateDefaultJob(catCommand, catParam);
    RunAndCheckNoAttachments(expectedCode, expectedMessage);

    QCOMPARE(job->GetCommandOutput().c_str(), expectedOutput.c_str());
}

wstring ConsoleJobTest::GetExpectedOkDescription()
{
    return L"";
}

wstring ConsoleJobTest::GetExpectedErrorDescription(const int, const int)
{
    return L"";
}

AbstractConsoleJob *ConsoleJobTest::CreateDefaultJob(void)
{
    return CreateDefaultJob(L"ls");
}

AbstractConsoleJob *ConsoleJobTest::CreateDefaultJob(const wstring &command,
                                                     const wstring &params)
{
    return new ConsoleJob(command, params);
}

void ConsoleJobTest::RunAndCheckNoAttachments(const int expectedCode,
                                              const std::wstring &expectedDescription)
{
    RunAndCheck(expectedCode, expectedDescription);
    CheckAttachmentCount(0, 0);
}

void ConsoleJobTest::RunAndCheckOneAttachment(const int expectedCode,
                                              const wstring &expectedDescription,
                                              const wstring &expectedAttachmentContent)
{
    RunAndCheck(expectedCode, expectedDescription);
    CheckAttachmentCount(0, 1);

    vector<pair<wstring,wstring> > buffers;
    status->GetFileBuffers(buffers);
    if (buffers.size() == 0)
        QFAIL("No attachment to check");
    else
        QCOMPARE(buffers.front().second.c_str(), expectedAttachmentContent.c_str());
}

void ConsoleJobTest::RunAndCheck(const int expectedCode,
                                 const wstring& expectedDescription)
{
    status = job->Run();
    QCOMPARE(status->GetCode(), expectedCode);
    QCOMPARE(status->GetDescription().c_str(), expectedDescription.c_str());
}

void ConsoleJobTest::CheckAttachmentCount(const unsigned long fileCount,
                                          const unsigned long bufferCount)
{
    vector<wstring> filenames;
    vector<pair<wstring, wstring> > filebuffers;
    status->GetExternalFilenames(filenames);
    status->GetFileBuffers(filebuffers);
    QCOMPARE(filenames.size(), fileCount);
    QCOMPARE(filebuffers.size(), bufferCount);
}

void ConsoleJobTest::TestCommandWithAppendedParameter()
{
    TestCommandWithParameter(JobStatus::Ok, L"", L"my super content", true);
}

void ConsoleJobTest::TestCommandWithSeparatedParameter()
{
    TestCommandWithParameter(JobStatus::Ok, L"", L"my super content", false);
}

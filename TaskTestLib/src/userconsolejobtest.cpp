#include "userconsolejobtest.h"

#include <QTest>
#include <sstream>

#include "filetools.h"
#include "testutils.h"

using namespace std;

UserConsoleJobTest::UserConsoleJobTest()
{
}

void UserConsoleJobTest::testRun_InvalidCommand()
{
    job = CreateDefaultJob(L"nonexistentcommand");

    RunAndCheckNoAttachments(JobStatus::Error, ConsoleJob::NotAvailableError);

    GetJob()->SetAttachOutput(true);

    // Even though job is marked to attach output, as command is invalid,
    // nothing is attached.
    RunAndCheckNoAttachments(JobStatus::Error, ConsoleJob::NotAvailableError);
}

void UserConsoleJobTest::testRun_CheckReturnCode()
{
    job = ConsoleJobTest::CreateDefaultJob();

    job->SetExpectedReturnCode(0);
    RunAndCheckNoAttachments(JobStatus::Ok, L"");

    job->SetExpectedReturnCode(1);
    RunAndCheckNoAttachments(JobStatus::Error, GetExpectedErrorDescription(1, 0));
}

void UserConsoleJobTest::testRun_CheckOutput()
{
    FileTools::WriteBufferToFile(L"testFile", L"test content");
    job = ConsoleJobTest::CreateDefaultJob();

    GetJob()->SetExpectedOutput(L"testFile");
    RunAndCheckNoAttachments(JobStatus::Ok, L"");

    GetJob()->SetExpectedOutput(L"Output is not that");
    RunAndCheckNoAttachments(JobStatus::Error, GetExpectedErrorDescription(L"Output is not that", L"testFile"));
}

void UserConsoleJobTest::testRun_CheckAttachment()
{
    FileTools::WriteBufferToFile(L"testFile", L"test content");
    job = ConsoleJobTest::CreateDefaultJob();

    RunAndCheckNoAttachments(JobStatus::Ok, L"");

    GetJob()->SetAttachOutput(true);
    RunAndCheckOneAttachment(JobStatus::Ok, L"", L"testFile");
}

void UserConsoleJobTest::testRun_OutputToFile_ReturnCode()
{
    FileTools::WriteBufferToFile(L"testFile", L"test content");
    job = CreateDefaultJob(L"ls",  L"testFile");
    GetJob()->SetOutputTofile(L"outputFile");

    RunAndCheck(JobStatus::Ok, L"");
    CheckAttachmentCount(1, 0);

    const wstring content = FileTools::GetTextFileContent(L"outputFile");
    TestUtils::Compare(content.c_str(), "testFile\n");
}

void UserConsoleJobTest::testRun_OutputToFile_OutputDoesNotWork()
{
    const wstring testFileName = L"testFile";
    const wstring testFileContent = L"test content";
    const wstring outputFileName = L"output";

    FileTools::WriteBufferToFile(testFileName, testFileContent);
    job = CreateDefaultJob(L"cat", testFileName);
    GetJob()->SetOutputTofile(outputFileName);

    GetJob()->SetExpectedOutput(testFileContent);
    RunAndCheck(JobStatus::Error, GetExpectedErrorDescription(testFileContent, L""));
    CheckAttachmentCount(1, 0);

    const wstring wrongContent = L"wrong content";
    GetJob()->SetExpectedOutput(wrongContent);
    RunAndCheck(JobStatus::Error, GetExpectedErrorDescription(wrongContent, L""));
    CheckAttachmentCount(1, 0);
}

void UserConsoleJobTest::testRun_AttachUserFile()
{
   const wstring testFileName = L"testFile.txt";
   const wstring testFileContent = L"test content";

   FileTools::WriteBufferToFile(testFileName, testFileContent);
   job = CreateDefaultJob(L"echo", L"blabla");
   GetJob()->AddUserAttachment(testFileName);

   RunAndCheck(JobStatus::Ok, L"");
   CheckAttachmentCount(1, 0);

   vector<wstring> userAttachment;
   status->GetExternalFilenames(userAttachment);

   if (userAttachment.size() == 1)
   {
      TestUtils::Compare(userAttachment.front(), testFileName);

      const wstring retrievedContent = FileTools::GetTextFileContent(testFileName);
      TestUtils::Compare(retrievedContent, testFileContent);
   }
   else
      QFAIL("Wrong number of file attachments");
}

void UserConsoleJobTest::testConfiguration_CheckConditions()
{
    job = ConsoleJobTest::CreateDefaultJob();
    CheckJobConditions(0, L"");

    GetJob()->SetExpectedReturnCode(2);
    CheckJobConditions(2, L"");

    GetJob()->SetExpectedOutput(L"blabla");
    CheckJobConditions(-1, L"blabla");

    GetJob()->SetExpectedReturnCode(3);
    CheckJobConditions(3, L"");
}

wstring UserConsoleJobTest::GetExpectedErrorDescription(const int expectedCode,
                                                        const int receivedCode)
{
    wstringstream stream;
    stream << "Return value : " << receivedCode << " - expected : " << expectedCode << endl;
    return stream.str();
}

wstring UserConsoleJobTest::GetExpectedErrorDescription(const wstring& expected,
                                                        const wstring& received)
{
    wstringstream stream;
    stream << "Received <" << received << "> - expected <" << expected << ">" << endl;
    return stream.str();
}

ConsoleJob *UserConsoleJobTest::CreateDefaultJob(const wstring& command,
                                                 const wstring& params)
{
    return new UserConsoleJob(L"dummyName", command, params);
}

UserConsoleJob *UserConsoleJobTest::GetJob()
{
    return dynamic_cast<UserConsoleJob*>(job);
}

void UserConsoleJobTest::CheckJobConditions(const int expectedCode,
                                            const wstring& expectedOutput)
{
    QCOMPARE(GetJob()->GetExpectedReturnCode(), expectedCode);
    QCOMPARE(GetJob()->GetExpectedOutput().c_str(), expectedOutput.c_str());
}

void UserConsoleJobTest::TestCommandWithAppendedParameter()
{
    TestCommandWithParameter(JobStatus::Error, ConsoleJob::NotAvailableError, L"", true);

}


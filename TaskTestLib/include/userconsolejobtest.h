#ifndef USERCONSOLEJOBTEST_H
#define USERCONSOLEJOBTEST_H

#include "consolejobtest.h"
#include "userconsolejob.h"

class UserConsoleJobTest : public ConsoleJobTest
{
    Q_OBJECT

public:
    UserConsoleJobTest();

private Q_SLOTS:
    void testRun_InvalidCommand();
    void testRun_CheckReturnCode();
    void testRun_CheckOutput();
    void testRun_CheckAttachment();
    void testRun_OutputToFile_ReturnCode();
    void testRun_OutputToFile_OutputDoesNotWork();
    void testRun_AttachUserFile();
    void testConfiguration_CheckConditions();

protected:
    virtual std::wstring GetExpectedErrorDescription(const int expectedCode,
                                                    const int receivedCode);
    std::wstring GetExpectedErrorDescription(const std::wstring& expected,
                                             const std::wstring& received);

    virtual ConsoleJob* CreateDefaultJob(const std::wstring& command,
                                         const std::wstring& params = L"") override;
    UserConsoleJob* GetJob(void);

    void CheckJobConditions(const int expectedCode,
                            const std::wstring& expectedOutput);

private:
    virtual void TestCommandWithAppendedParameter() override;

};

#endif // USERCONSOLEJOBTEST_H

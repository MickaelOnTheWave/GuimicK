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
    virtual std::string GetExpectedErrorDescription(const int expectedCode,
                                                    const int receivedCode);
    std::string GetExpectedErrorDescription(const std::string& expected,
                                            const std::string& received);

    virtual ConsoleJob* CreateDefaultJob(const std::string& command,
                                         const std::string& params = "") override;
    UserConsoleJob* GetJob(void);

    void CheckJobConditions(const int expectedCode,
                            const std::string& expectedOutput);

private:
    virtual void TestCommandWithAppendedParameter() override;

};

#endif // USERCONSOLEJOBTEST_H

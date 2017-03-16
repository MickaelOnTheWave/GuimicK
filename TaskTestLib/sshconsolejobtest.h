#ifndef SSHCONSOLEJOBTEST_H
#define SSHCONSOLEJOBTEST_H

#include "consolejobtest.h"
#include "sshconsolejob.h"

class SshConsoleJobTest : public ConsoleJobTest
{
    Q_OBJECT

public:
    SshConsoleJobTest();

private Q_SLOTS:

    void testRun_NoTargetError();
    void testRun_InvalidTargetError();
    void testRun_CheckResultingCommand();

protected:
    virtual AbstractConsoleJob* CreateDefaultJob(const std::string& command) override;

    SshConsoleJob* GetJob();

private:
    void CreateRemoteCatJob(const std::string& fileToTest);

};

#endif // SSHCONSOLEJOBTEST_H

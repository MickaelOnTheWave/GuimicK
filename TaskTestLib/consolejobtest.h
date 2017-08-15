#ifndef CONSOLEJOBTEST_H
#define CONSOLEJOBTEST_H

#include <QObject>

#include "consolejob.h"

class ConsoleJobTest : public QObject
{
    Q_OBJECT

public:
    ConsoleJobTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testRunOk();
    void testRunError();
    void testCommandWithParameter();

protected:
    virtual std::string GetExpectedOkDescription();
    virtual std::string GetExpectedErrorDescription(const int expectedCode,
                                                    const int receivedCode);

    virtual AbstractConsoleJob* CreateDefaultJob(void);
    virtual AbstractConsoleJob* CreateDefaultJob(const std::string& command,
                                                 const std::string& params = "");

    void RunAndCheckNoAttachments(const int expectedCode,
                                  const std::string& expectedDescription);
    void RunAndCheckOneAttachment(const int expectedCode,
                                  const std::string &expectedDescription,
                                  const std::string &expectedAttachmentContent);

    void RunAndCheck(const int expectedCode,
                     const std::string& expectedDescription);
    void CheckAttachmentCount(const unsigned long fileCount,
                              const unsigned long bufferCount);

    AbstractConsoleJob* job = nullptr;
    JobStatus* status = nullptr;
};

#endif // CONSOLEJOBTEST_H

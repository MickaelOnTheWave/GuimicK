#ifndef CONSOLEJOBTEST_H
#define CONSOLEJOBTEST_H

#include <QObject>

#include "consolejob.h"

// TODO : remove this and find a way to use JobStatus values outside of library
#define JobStatus_NOT_EXECUTED      0
#define JobStatus_OK                1
#define JobStatus_OK_WITH_WARNINGS  2
#define JobStatus_ERROR             3

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

protected:
    virtual std::string GetExpectedOkDescription();
    virtual std::string GetExpectedErrorDescription(const int expectedCode,
                                                    const int receivedCode);

    virtual ConsoleJob* CreateDefaultJob(void);
    virtual ConsoleJob* CreateDefaultJob(const std::string& command);

    void RunAndCheckNoAttachments(const int expectedCode,
                                  const std::string& expectedDescription);
    void RunAndCheck(const int expectedCode,
                     const std::string& expectedDescription);
    void CheckAttachmentCount(const unsigned long fileCount,
                              const unsigned long bufferCount);

    ConsoleJob* job = nullptr;
    JobStatus* status = nullptr;
};

#endif // CONSOLEJOBTEST_H

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

    void testRun_InvalidCommand();
    void testRun_CheckReturnCode();
    void testRun_CheckOutput();
    void testRun_CheckAttachment();
    void testRun_OutputToFile();

private:
    void RunAndCheckNoAttachments(const int expectedCode,
                                  const std::string& expectedDescription);
    void RunAndCheckWithAttachment(const int expectedCode,
                                   const std::string& expectedDescription);
    void RunAndCheck(const int expectedCode,
                     const std::string& expectedDescription);
    void CheckAttachmentCount(const unsigned long fileCount,
                              const unsigned long bufferCount);

    ConsoleJob* job = nullptr;
    JobStatus* status = nullptr;
};

#endif // CONSOLEJOBTEST_H

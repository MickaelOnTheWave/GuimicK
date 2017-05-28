#ifndef COPYFSBACKUPJOBTEST_H
#define COPYFSBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

class CopyFsBackupJobTest : public AbstractFsBackupJobTest
{
    Q_OBJECT

public:
    CopyFsBackupJobTest(const std::string& dataPrefix);
    virtual ~CopyFsBackupJobTest();

private Q_SLOTS:
    void testRunBackup_data();

protected:
    virtual void CheckBackedUpDataIsOk();
    virtual JobStatus* RunBackupJob();
};

#endif // COPYFSBACKUPJOBTEST_H

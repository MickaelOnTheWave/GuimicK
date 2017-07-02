#ifndef ABSTRACTCOPYFSBACKUPJOBTEST_H
#define ABSTRACTCOPYFSBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

#include "abstractcopyfsbackupjob.h"

class AbstractCopyFsBackupJobTest : public AbstractFsBackupJobTest
{
    Q_OBJECT

public:
    AbstractCopyFsBackupJobTest(const std::string& dataPrefix,
                                const std::string& errorPrefix);
    virtual ~AbstractCopyFsBackupJobTest();

private Q_SLOTS:
    void testRunBackup_data();

protected:
    virtual void CheckBackedUpDataIsOk();
    virtual JobStatus* RunBackupJob();

    virtual AbstractCopyFsBackupJob* CreateCopyJob() = 0;
};

#endif // COPYFSBACKUPJOBTEST_H

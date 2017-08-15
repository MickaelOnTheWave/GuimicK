#ifndef ZIPANDCOPYFSBACKUPJOBTEST_H
#define ZIPANDCOPYFSBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

class ZipAndCopyFsBackupJobTest : public AbstractFsBackupJobTest
{
    Q_OBJECT

public:
    ZipAndCopyFsBackupJobTest(const std::string& dataPrefix, const std::string& errorPrefix);
    virtual ~ZipAndCopyFsBackupJobTest();

private Q_SLOTS:
    void testRunBackup_data();

protected:
    virtual void CheckBackedUpDataIsOk() override;
    virtual JobStatus* RunBackupJob(const bool isRemote,
                                    const bool useDebug) override;
    virtual AbstractBackupJob* CreateNewJob() override;

};

#endif // ZIPANDCOPYFSBACKUPJOBTEST_H

#ifndef ZIPANDCOPYFSBACKUPJOBTEST_H
#define ZIPANDCOPYFSBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

class ZipAndCopyFsBackupJobTest : public AbstractFsBackupJobTest
{
    Q_OBJECT

public:
    ZipAndCopyFsBackupJobTest(const std::wstring& dataPrefix,
                              const std::wstring& errorPrefix);
    virtual ~ZipAndCopyFsBackupJobTest() = default;

private Q_SLOTS:
    void testRunBackup_data();

protected:
    virtual void CheckBackedUpDataIsOk() override;
    JobStatus* RunBackupJob(const bool isRemote,
                            const bool useDebug) override;

    std::wstring GetBackupDestination() const override;

    AbstractBackupJob* CreateNewJob() override;
};

#endif // ZIPANDCOPYFSBACKUPJOBTEST_H

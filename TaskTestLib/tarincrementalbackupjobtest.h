#ifndef TARINCREMENTALBACKUPJOBTEST_H
#define TARINCREMENTALBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

// TODO : remove duplication and ZipAndCopy test suite
class TarIncrementalBackupJobTest : public AbstractFsBackupJobTest
{
   Q_OBJECT

public:
    TarIncrementalBackupJobTest( const std::string& dataPrefix,
                                 const std::string &errorPrefix);
    virtual ~TarIncrementalBackupJobTest() = default;

private Q_SLOTS:
    void testRunBackup_data();

protected:
    virtual void CheckBackedUpDataIsOk() override;
    virtual JobStatus* RunBackupJob(const bool isRemote,
                                    const bool useDebug) override;

    virtual std::string GetBackupDestination() const override;

    virtual AbstractBackupJob* CreateNewJob();

};

#endif // TARINCREMENTALBACKUPJOBTEST_H

#ifndef GITFSBACKUPJOBTEST_H
#define GITFSBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

class GitFsBackupJobTest : public AbstractFsBackupJobTest
{
    Q_OBJECT

public:
    GitFsBackupJobTest(const std::string& dataPrefix);
    virtual ~GitFsBackupJobTest();

private Q_SLOTS:
    void testRunBackup_data();

    //void testRunBackupWithDebug();

protected:
    void LoadCustomDataSamples(const bool isRemote);

    virtual void CheckBackedUpDataIsOk();
    virtual JobStatus* RunBackupJob();
};

#endif // GITFSBACKUPJOBTEST_H

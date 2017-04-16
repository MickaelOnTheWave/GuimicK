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

protected:
    virtual void CheckBackedUpDataIsOk();
    virtual JobStatus* RunBackupJob();

private:
    bool Restore(const std::string& repository, const std::string& destination);

};

#endif // GITFSBACKUPJOBTEST_H

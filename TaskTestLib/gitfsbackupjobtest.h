#ifndef GITFSBACKUPJOBTEST_H
#define GITFSBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"
#include "gitfsbackupjob.h"

class GitFsBackupJobTest : public AbstractFsBackupJobTest
{
    Q_OBJECT

public:
    GitFsBackupJobTest(const std::string& dataPrefix,
                       const std::string &errorPrefix);
    virtual ~GitFsBackupJobTest();

private Q_SLOTS:
    void testRunBackup_data();

protected:
    virtual AbstractBackupJob* CreateNewJob();
};

#endif // GITFSBACKUPJOBTEST_H

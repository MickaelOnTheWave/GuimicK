#ifndef GITFSBACKUPJOBTEST_H
#define GITFSBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

class GitFsBackupJobTest : public AbstractFsBackupJobTest
{
    Q_OBJECT

public:
    GitFsBackupJobTest(const std::wstring& dataPrefix,
                       const std::wstring& errorPrefix);
    virtual ~GitFsBackupJobTest() = default;

private Q_SLOTS:
    void testRunBackup_data();

protected:
    virtual AbstractBackupJob* CreateNewJob();
};

#endif // GITFSBACKUPJOBTEST_H

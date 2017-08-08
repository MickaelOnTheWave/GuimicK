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

    void testCreatesOnlyOneAttachment();
    void testCreatesDebugAttachment();

protected:
    virtual void CheckBackedUpDataIsOk();
    virtual JobStatus* RunBackupJob();

private:
    void testCheckJobAttachments(const bool debugOutput,
                                 const std::vector<std::string>& expectedAttachments);

    JobStatus* RunBackupJob(GitFsBackupJob* job, const bool isRemote);
};

#endif // GITFSBACKUPJOBTEST_H

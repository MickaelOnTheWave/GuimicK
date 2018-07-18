#ifndef RSYNCCOPYFSBACKUPJOBTEST_H
#define RSYNCCOPYFSBACKUPJOBTEST_H

#include "abstractcopyfsbackupjobtest.h"

class RsyncCopyFsBackupJobTest : public AbstractCopyFsBackupJobTest
{
    Q_OBJECT

public:
    RsyncCopyFsBackupJobTest(const std::string& dataPrefix,
                             const std::string& errorPrefix);
    virtual ~RsyncCopyFsBackupJobTest();

protected:
    virtual AbstractCopyFsBackupJob* CreateNewJob();

};
#endif // RSYNCCOPYFSBACKUPJOBTEST_H

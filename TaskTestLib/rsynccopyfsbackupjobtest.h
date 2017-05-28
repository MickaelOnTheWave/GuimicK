#ifndef RSYNCCOPYFSBACKUPJOBTEST_H
#define RSYNCCOPYFSBACKUPJOBTEST_H

#include "abstractcopyfsbackupjobtest.h"

class RsyncCopyFsBackupJobTest : public AbstractCopyFsBackupJobTest
{
    Q_OBJECT

public:
    RsyncCopyFsBackupJobTest(const std::string& dataPrefix);
    virtual ~RsyncCopyFsBackupJobTest();

protected:
    virtual AbstractCopyFsBackupJob* CreateCopyJob();

};
#endif // RSYNCCOPYFSBACKUPJOBTEST_H

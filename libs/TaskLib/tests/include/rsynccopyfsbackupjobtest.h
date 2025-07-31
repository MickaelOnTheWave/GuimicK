#ifndef RSYNCCOPYFSBACKUPJOBTEST_H
#define RSYNCCOPYFSBACKUPJOBTEST_H

#include "abstractcopyfsbackupjobtest.h"

class RsyncCopyFsBackupJobTest : public AbstractCopyFsBackupJobTest
{
    Q_OBJECT

public:
    RsyncCopyFsBackupJobTest(const std::wstring& dataPrefix,
                             const std::wstring& errorPrefix);
    virtual ~RsyncCopyFsBackupJobTest();

protected:
    virtual AbstractCopyFsBackupJob* CreateNewJob();

};
#endif // RSYNCCOPYFSBACKUPJOBTEST_H

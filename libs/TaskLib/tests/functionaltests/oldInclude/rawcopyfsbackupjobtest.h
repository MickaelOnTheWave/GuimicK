#ifndef RAWCOPYFSBACKUPJOBTEST_H
#define RAWCOPYFSBACKUPJOBTEST_H

#include "abstractcopyfsbackupjobtest.h"

class RawCopyFsBackupJobTest : public AbstractCopyFsBackupJobTest
{
    Q_OBJECT

public:
    RawCopyFsBackupJobTest(const std::wstring& dataPrefix,
                           const std::wstring &errorPrefix);
    virtual ~RawCopyFsBackupJobTest();

protected:
    virtual AbstractBackupJob* CreateNewJob();

};

#endif // RAWCOPYFSBACKUPJOBTEST_H

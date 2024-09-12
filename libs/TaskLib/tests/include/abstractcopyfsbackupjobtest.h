#ifndef ABSTRACTCOPYFSBACKUPJOBTEST_H
#define ABSTRACTCOPYFSBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

class AbstractCopyFsBackupJobTest : public AbstractFsBackupJobTest
{
public:
    AbstractCopyFsBackupJobTest(const std::wstring& dataPrefix,
                                const std::wstring& errorPrefix);
    virtual ~AbstractCopyFsBackupJobTest();

private:
    void testRunBackup_data();
};

#endif // COPYFSBACKUPJOBTEST_H

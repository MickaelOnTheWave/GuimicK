#ifndef ABSTRACTCOPYFSBACKUPJOBTEST_H
#define ABSTRACTCOPYFSBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

#include "abstractcopyfsbackupjob.h"

class AbstractCopyFsBackupJobTest : public AbstractFsBackupJobTest
{
    Q_OBJECT

public:
    AbstractCopyFsBackupJobTest(const std::wstring& dataPrefix,
                                const std::wstring& errorPrefix);
    virtual ~AbstractCopyFsBackupJobTest();

private Q_SLOTS:
    void testRunBackup_data();
};

#endif // COPYFSBACKUPJOBTEST_H

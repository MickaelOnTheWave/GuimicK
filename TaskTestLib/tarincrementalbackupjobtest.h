#ifndef TARINCREMENTALBACKUPJOBTEST_H
#define TARINCREMENTALBACKUPJOBTEST_H

#include "abstractfsbackupjobtest.h"

class TarIncrementalBackupJobTest : public AbstractFsBackupJobTest
{
   Q_OBJECT

public:
    TarIncrementalBackupJobTest( const std::string& dataPrefix,
                                 const std::string &errorPrefix);
    virtual ~TarIncrementalBackupJobTest() = default;

private Q_SLOTS:
    void testRunBackup_data();

protected:
    virtual AbstractBackupJob* CreateNewJob();

};

#endif // TARINCREMENTALBACKUPJOBTEST_H

#ifndef TARINCREMENTALBACKUPJOBTEST_H
#define TARINCREMENTALBACKUPJOBTEST_H

#include "qttestsuite.h"

#include "abstractbackupjob.h"

// TODO : remove duplication and ZipAndCopy test suite
class TarIncrementalBackupJobTest : public QtTestSuite
{
   Q_OBJECT

public:
    TarIncrementalBackupJobTest( const std::string& dataPrefix,
                                 const std::string &errorPrefix);
    virtual ~TarIncrementalBackupJobTest() = default;

private Q_SLOTS:
    void testRunBackup_Added();
    void testRunBackup_Modified();
    //void testRunBackup_Removed();
    void testRunBackup_Mixed();

private:
    void CreateInitialData();

    void RunInitialBackup(AbstractBackupJob* job);

    AbstractBackupJob* CreateInitializedJob();

};

#endif // TARINCREMENTALBACKUPJOBTEST_H

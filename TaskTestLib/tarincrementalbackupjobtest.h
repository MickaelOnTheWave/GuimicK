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
                                 const std::string& errorPrefix);
    virtual ~TarIncrementalBackupJobTest() = default;

    void testRunBackup_Added();
    void testRunBackup_Modified();
    void testRunBackup_Mixed();


private Q_SLOTS:
    void init();
    void cleanup();

    void testBackupAndRestoreMultipleLevels();

private:
    void testRunBackup(const QStringList& toAdd, const QStringList& toModify);

    JobStatus* RunBackupStage(AbstractBackupJob* job,
                              const QStringList& toAdd, const QStringList& toModify);
    void RunBackupStageWithoutStatus(AbstractBackupJob* job,
                                     const QStringList& toAdd, const QStringList& toModify);

    void CreateInitialData();

    void RunInitialBackup(AbstractBackupJob* job);

    AbstractBackupJob* CreateInitializedJob();

    void AddFiles(const QStringList& filesToAdd);

    void ModifyFiles(const QStringList& filesToModify);

    void CheckStatus(JobStatus* status, const QStringList& addedFiles,
                     const QStringList& modifiedFiles);

    void CheckReport(JobStatus* status, const FileBackupReport& expectedReport);

    void RestoreAndCheck(AbstractBackupJob* job,
                         const int timeIndex,
                         const QStringList& expectedFiles);

};

#endif // TARINCREMENTALBACKUPJOBTEST_H

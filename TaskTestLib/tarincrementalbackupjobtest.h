#ifndef TARINCREMENTALBACKUPJOBTEST_H
#define TARINCREMENTALBACKUPJOBTEST_H

#include "qttestsuite.h"

#include "abstractbackupjob.h"

// TODO : remove duplication and ZipAndCopy test suite
class TarIncrementalBackupJobTest : public QtTestSuite
{
   Q_OBJECT

public:
    TarIncrementalBackupJobTest();
    virtual ~TarIncrementalBackupJobTest() = default;

private Q_SLOTS:
    void init();
    void cleanup();

    void testRunBackup_Added();
    void testRunBackup_Modified();
    void testRunBackup_Mixed();
    void testBackupAndRestoreMultipleLevels();

private:
    void testRunBackup(const QStringList& addNames, const QStringList& addContents,
                       const QStringList& modifyNames, const QStringList& modifyContents);

    JobStatus* RunBackupStage(AbstractBackupJob* job,
                              const QStringList& addNames,
                              const QStringList& addContents,
                              const QStringList& modifyNames,
                              const QStringList& modifyContents);
    void RunBackupStageWithoutStatus(AbstractBackupJob* job,
                                     const QStringList& addNames,
                                     const QStringList& addContents,
                                     const QStringList& modifyNames,
                                     const QStringList& modifyContents);

    void CreateInitialData();

    void RunInitialBackup(AbstractBackupJob* job);

    AbstractBackupJob* CreateInitializedJob();

    void AddFiles(const QStringList& names,
                  const QStringList& contents);

    void ModifyFiles(const QStringList& names,
                     const QStringList& contents);

    void CheckStatus(JobStatus* status, const QStringList& addedFiles,
                     const QStringList& modifiedFiles);

    void CheckReport(JobStatus* status, const FileBackupReport& expectedReport);

    void RestoreAndCheck(AbstractBackupJob* job,
                         const int timeIndex,
                         const QStringList& expectedFiles,
                         const QStringList& expectedContents);

};

#endif // TARINCREMENTALBACKUPJOBTEST_H

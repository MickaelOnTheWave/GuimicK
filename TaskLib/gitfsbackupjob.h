#ifndef GITFSBACKUPJOB_H
#define GITFSBACKUPJOB_H

#include "abstractbackupjob.h"
#include "abstractcopyfsbackupjob.h"
#include "backupstatusmanager.h"
#include "filebackupreport.h"
#include "jobdebuginformationmanager.h"

class GitFsBackupJob : public AbstractBackupJob
{
public:
    GitFsBackupJob();
    GitFsBackupJob(const GitFsBackupJob& other);

    virtual std::string GetName();
    virtual AbstractJob* Clone();
    virtual JobStatus* Run();

    void SetJoinAllBackups(const bool value);
    void SetForceRawCopyUse(const bool value);

private:
    void RunRepositoryBackup(const std::string& source,
                             const std::string& destination,
                             ResultCollection &results);
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);

    void CreateGitRepository(const std::string& path, JobStatus* status);
    void CleanDestination(const std::string& destination, JobStatus* status);
    void CopyData(const std::string& source, const std::string& destination,
                       JobStatus* status);
    void AddData(JobStatus* status);
    std::string CommitData(JobStatus* status);
    bool HasChangesInRepository() const;
    void CreateReport(const std::string &commitId, JobStatus* status, FileBackupReport &report);

    void FixCRLFIssue();
    void SetGitUserToFixUtf8Warning();

    int GetRevisionCount() const;
    void CreateInitialReport(JobStatus* status, FileBackupReport& report);
    void CreateDifferentialReport(const std::string &commitId, JobStatus* status,
                                  FileBackupReport& report);

    std::string GetCommitId(const std::string& output);

    bool IsGitInstalled() const;

    bool IsCommitCodeOk(const int code) const;

    AbstractCopyFsBackupJob* PrepareCopy(const std::string& destination, JobStatus* status);
    void RunCopy(AbstractCopyFsBackupJob* copyJob,
                 const std::string& source, const std::string& destination, JobStatus* status);

    BackupStatusManager statusManager;
    bool forceRawCopy;
};

#endif // GITFSBACKUPJOB_H

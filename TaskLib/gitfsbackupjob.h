#ifndef GITFSBACKUPJOB_H
#define GITFSBACKUPJOB_H

#include "abstractbackupjob.h"
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

    void SetOutputDebugInformation(const bool value);
    void SetJoinAllBackups(const bool value);

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

    int RunCopyCommand(const std::string& source, const std::string& destination);

    void FixCRLFIssue();
    void SetGitUserToFixUtf8Warning();

    int GetRevisionCount() const;
    void CreateInitialReport(JobStatus* status, FileBackupReport& report);
    void CreateDifferentialReport(const std::string &commitId, JobStatus* status,
                                  FileBackupReport& report);

    std::string GetCommitId(const std::string& output);

    bool IsGitInstalled() const;

    bool IsCommitCodeOk(const int code) const;

    BackupStatusManager statusManager;
};

#endif // GITFSBACKUPJOB_H

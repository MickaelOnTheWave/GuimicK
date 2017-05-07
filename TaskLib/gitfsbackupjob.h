#ifndef GITFSBACKUPJOB_H
#define GITFSBACKUPJOB_H

#include "abstractbackupjob.h"

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
    JobStatus *RunRepositoryBackup( const std::string& source,
                                    const std::string& destination);
    JobStatus* CreateGlobalStatus(std::vector<JobStatus*>& statuses);
    void CreateGitRepository(const std::string& path, JobStatus* status);
    void CleanDestination(const std::string& destination, JobStatus* status);
    void CopyData(const std::string& source, const std::string& destination,
                       JobStatus* status);
    void AddData(JobStatus* status);
    std::string CommitData(JobStatus* status);
    void CreateReport(const std::string &commitId, JobStatus* status);

    int RunCopyCommand(const std::string& source, const std::string& destination);

    int GetRevisionCount() const;
    void CreateInitialReport(JobStatus* status);
    void CreateDifferentialReport(const std::string &commitId, JobStatus* status);

    std::string GetCommitId(const std::string& output);

    bool IsGitInstalled() const;

    JobDebugInformationManager debugManager;
    bool usingDebugInformation;
    bool joinRepositoriesReports;
};

#endif // GITFSBACKUPJOB_H

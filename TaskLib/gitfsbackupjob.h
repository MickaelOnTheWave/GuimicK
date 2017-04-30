#ifndef GITFSBACKUPJOB_H
#define GITFSBACKUPJOB_H

#include "abstractjob.h"

class GitFsBackupJob : public AbstractJob
{
public:
    GitFsBackupJob();
    GitFsBackupJob(const GitFsBackupJob& other);

    virtual std::string GetName();
    virtual AbstractJob* Clone();
    virtual bool InitializeFromClient(Client* client);
    virtual bool IsInitialized(void);
    virtual JobStatus* Run();

    // TODO : check if redundancy with gitbackupjob can be removed
    void SetTargetRemote(const std::string& user = "", const std::string& host = "");
    void SetTargetLocal();

    void AddFolder(const std::string& folder, const std::string& repository);

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

    bool IsValidRemoteTarget() const;

    int GetRevisionCount() const;
    void CreateInitialReport(JobStatus* status);
    void CreateDifferentialReport(const std::string &commitId, JobStatus* status);

    std::string GetCommitId(const std::string& output);

    std::vector<std::pair<std::string, std::string> > repositoryList;

    std::string sshUser;
    std::string sshHost;
    bool isTargetLocal;
};

#endif // GITFSBACKUPJOB_H

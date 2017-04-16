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

    void AddFolder(const std::string& folder, const std::string& repository);

private:
    JobStatus *RunRepositoryBackup( const std::string& source,
                                    const std::string& destination);
    JobStatus* CreateGlobalStatus(std::vector<JobStatus*>& statuses);
    void CreateGitRepository(const std::string& path, JobStatus* status);
    void CleanDestination(const std::string& destination, JobStatus* status);
    void CopyData(const std::string& source, const std::string& destination,
                       JobStatus* status);
    void AddData(const std::string& destination, JobStatus* status);
    void CommitData(const std::string& destination, JobStatus* status);

    std::vector<std::pair<std::string, std::string> > repositoryList;
};

#endif // GITFSBACKUPJOB_H

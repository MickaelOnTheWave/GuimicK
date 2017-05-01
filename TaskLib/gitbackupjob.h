#ifndef GITBACKUPJOB_H
#define GITBACKUPJOB_H

#include "abstractbackupjob.h"

class GitBackupJob : public AbstractBackupJob
{
public:
    GitBackupJob();
    GitBackupJob(const GitBackupJob& other);
    GitBackupJob(const std::vector<std::pair<std::string, std::string> >& repositoryList);
    virtual ~GitBackupJob();

    virtual std::string GetName();
    virtual AbstractJob* Clone();
    virtual JobStatus* Run();

    void SetWriteLogsToFiles(bool enabled);

private:
    bool InitializeRemoteTarget(Client* client);
    bool AreSourcesConsistent() const;
    void RunGitRepositoryBackup(const std::string &source,
                                const std::string &destination,
                                std::vector<JobStatus *> &statusList) const;
    void RunGitPull(const std::string &repository,
                    std::vector<JobStatus *> &statusList) const;
    void RunGitClone(const std::string &source,
                     const std::string &destination,
                     std::vector<JobStatus *> &statusList) const;
    std::string BuildGitParameters(const std::string &source, const std::string &destination) const;
    JobStatus* CreateGlobalStatus(const std::vector<JobStatus*>& statusList) const;
    JobStatus* CreateSingleRepositoryStatus(JobStatus *status) const;
    JobStatus* CreateMultiRepositoryStatus(const std::vector<JobStatus*>& statusList) const;

    unsigned int CountFaultyRepositories(const std::vector<JobStatus*>& statusList) const;
    std::string GetCorrectRepositoryWord() const;

    bool writeLogsToFile;
};

#endif // GITBACKUPJOB_H

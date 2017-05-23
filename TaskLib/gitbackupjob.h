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

    void SetWriteLogsToFiles(bool enabled);

protected:
    virtual void RunRepositoryBackup(const std::string &source,
                                     const std::string &destination,
                                     ResultCollection &results);
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);

private:
    bool InitializeRemoteTarget(Client* client);
    bool AreSourcesConsistent() const;
    void RunGitPull(const std::string &repository,
                    ResultCollection& statusList) const;
    void RunGitClone(const std::string &source,
                     const std::string &destination,
                     ResultCollection& statusList) const;
    std::string BuildGitParameters(const std::string &source, const std::string &destination) const;
    JobStatus* CreateGlobalStatus(const std::vector<JobStatus*>& statusList) const;
    JobStatus* CreateSingleRepositoryStatus(const ResultEntry &entry) const;
    JobStatus* CreateMultiRepositoryStatus(const ResultCollection &results) const;

    unsigned int CountFaultyRepositories(const ResultCollection &results) const;
    std::string GetCorrectRepositoryWord() const;

    bool writeLogsToFile;
};

#endif // GITBACKUPJOB_H

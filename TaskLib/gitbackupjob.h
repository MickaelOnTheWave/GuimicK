#ifndef GITBACKUPJOB_H
#define GITBACKUPJOB_H

#include "abstractbackupjob.h"

#include "consolejob.h"

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

    void UpdateGitRepository(const std::string &repository,
                             ResultCollection& statusList);

    std::string GetRepositoryHeadId() const;

    bool FetchUpdates(const std::string &repository,
                      ResultCollection& statusList);

    void ComputeChanges(const std::string &repository,
                        const std::string& oldCommitId, const std::string& newCommitId,
                        ResultCollection& statusList);

    void CreateReport(const std::string &repository,
                      const std::string& commandOutput,
                      ResultCollection& statusList);

    void AddToAttachedArchive(const std::string& repository,
                              const std::string& content);

    void RunGitClone(const std::string &source,
                     const std::string &destination,
                     ResultCollection& statusList) const;
    std::string BuildGitParameters(const std::string &source, const std::string &destination) const;
    JobStatus* CreateGlobalStatus(const std::vector<JobStatus*>& statusList) const;
    JobStatus* CreateSingleRepositoryStatus(const ResultEntry &entry) const;
    JobStatus* CreateMultiRepositoryStatus(const ResultCollection &results) const;

    unsigned int CountFaultyRepositories(const ResultCollection &results) const;
    std::string GetCorrectRepositoryWord() const;

    bool IsInvalidSourceError(const ConsoleJob &job) const;

    bool writeLogsToFile;
    std::string archiveContent;
};

#endif // GITBACKUPJOB_H

#ifndef GITBACKUPJOB_H
#define GITBACKUPJOB_H

#include "abstractbackupjob.h"

#include "consolejob.h"

class GitBackupJob : public AbstractBackupJob
{
public:
    GitBackupJob();
    GitBackupJob(const GitBackupJob& other);
    GitBackupJob(const std::vector<std::pair<std::wstring, std::wstring> >& repositoryList);
    virtual ~GitBackupJob();

    virtual AbstractJob* Clone();

    virtual std::wstring GetTypeName() const;

    bool GetWriteLogsToFile() const;
    void SetWriteLogsToFiles(bool enabled);

protected:
    virtual void RunRepositoryBackup(const std::wstring &source,
                                     const std::wstring &destination,
                                     ResultCollection &results);

private:
    virtual JobStatus* RestoreBackupFromServer(const std::wstring &source, const std::wstring &destination);

    bool InitializeRemoteTarget(Client* client);
    bool AreSourcesConsistent() const;

    void UpdateGitRepositoryIfPossible(const std::wstring &gitRepository,
                                       ResultCollection& statusList);

    void UpdateGitRepository(const std::wstring &gitRepository,
                             ResultCollection& statusList);

    std::wstring GetRepositoryHeadId() const;

    bool FetchUpdates(const std::wstring &repository,
                      ResultCollection& statusList);

    void ComputeChanges(const std::wstring &repository,
                        const std::wstring& oldCommitId, const std::wstring& newCommitId,
                        ResultCollection& statusList);

    void CreateReport(const std::wstring &repository,
                      const std::wstring& commandOutput,
                      ResultCollection& statusList);

    void AddToAttachedArchive(const std::wstring& repository,
                              const std::wstring& content);

    void RunGitClone(const std::wstring &source,
                     const std::wstring &destination,
                     ResultCollection& statusList);
    std::wstring BuildCloneParameters(const std::wstring &source, const std::wstring &destination,
                                     const bool mirror) const;

    bool IsInvalidSourceError(const ConsoleJob &job) const;

    bool IsRemoteValid(const std::wstring &remoteUrl) const;

    std::wstring GetRemoteUrl(const std::wstring &destination) const;

    BackupJobStatus CreateWrongRemoteError(const std::wstring& remoteUrl) const;

    bool writeLogsToFile;
    std::wstring archiveContent;
};

#endif // GITBACKUPJOB_H

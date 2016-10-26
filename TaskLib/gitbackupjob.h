#ifndef GITBACKUPJOB_H
#define GITBACKUPJOB_H

#include "abstractjob.h"

/**
 * @brief Base class for git based backups
 */
class GitBackupJob : public AbstractJob
{
public:
    GitBackupJob(const std::vector<std::pair<std::string, std::string> >& _gitRepositoryList);
    virtual ~GitBackupJob();

    virtual std::string GetName();

    virtual bool InitializeFromClient(Client* client);
    virtual bool IsInitialized(void);
    virtual JobStatus* Run();

    void SetTargetRemote(const std::string& user = "", const std::string& host = "");
    void SetTargetLocal();
    void AddRepository(const std::string& sourcePath, const std::string& destPath);
    void ClearRepositoryList(void);


protected:
    std::string CorrectRepositoryWord(int n);

private:
    bool InitializeRemoteTarget(Client* client);
    bool IsRemoteTargetConsistent();
    bool AreSourcesConsistent() const;

    std::vector<std::pair<std::string, std::string> > gitRepositoryList;
    std::string sshUser;
    std::string sshHost;
    bool isTargetLocal;
};

#endif // GITBACKUPJOB_H

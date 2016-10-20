#ifndef GITCLONEJOB_H
#define GITCLONEJOB_H

#include "gitbackupjob.h"
#include "consolejob.h"

/**
 * @brief Does a backup of a git repository.
 * The "backup" is in fact a clone of the original repository.
 * It is generated using the "git clone" command.
 */
class GitCloneJob : public GitBackupJob
{
public:
    GitCloneJob(std::vector<std::pair<std::string, std::string> >* _gitRepositoryList,
                std::string _sshUser = "", std::string _sshHost = "");
    virtual ~GitCloneJob();

    virtual std::string GetName();

    virtual bool InitializeFromClient(Client*client);

    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    void SetRemoteTarget(const std::string& user = "", const std::string& host = "");
    void SetLocalTarget();

    void AddRepository(const std::string& sourcePath, const std::string& destPath);
    void ClearRepositoryList(void);

private:
    bool AreSourcesConsistent() const;
    std::string BuildGitParameters(const std::string &source, const std::string &destination) const;

    std::vector<std::pair<std::string, std::string> > gitRepositoryList;
    std::string sshUser;
    std::string sshHost;
    bool localTarget;
    ConsoleJob* gitCommand;
};

#endif // GITCLONEJOB_H

#ifndef GITPULLJOB_H
#define GITPULLJOB_H

#include "gitbackupjob.h"
#include "consolejob.h"

/**
 * @brief Does a backup of a git repository.
 * The "backup" is in fact a clone of the original repository. This
 * repository must already exist and be a normal clone (not bare).
 * The update is in fact a "git pull" command - so there must already
 * be a pull target configured and the repository must be valid.
 */
class GitPullJob : public GitBackupJob
{
public:
    GitPullJob(std::vector<std::string>* _gitRepositoryList);
    virtual ~GitPullJob();

    virtual std::string GetName();

    virtual bool InitializeFromClient(Client*);

    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    void AddRepository(std::string& repositoryPath);
    void ClearRepositoryList(void);

private:
    ConsoleJob* gitCommand;

    std::vector<std::string> gitRepositoryList;
};

#endif // GITPULLJOB_H

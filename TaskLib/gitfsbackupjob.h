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
};

#endif // GITFSBACKUPJOB_H

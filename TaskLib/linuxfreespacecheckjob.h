#ifndef LINUXFREESPACECHECKJOB_H
#define LINUXFREESPACECHECKJOB_H

#include "abstractjob.h"

#include "consolejob.h"

class LinuxFreeSpaceCheckJob : public AbstractJob
{
public:
    LinuxFreeSpaceCheckJob();
    LinuxFreeSpaceCheckJob(const std::string& _drive);


    virtual std::string GetName();

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    bool IsTargetLocal() const;
    void SetTargetToLocal(const bool value);

    std::string GetDrive() const;
    void SetDrive(const std::string& value);


private:
    bool IsRemoteTargetConsistent() const;
    AbstractConsoleJob *CreateJobInstance() const;

    std::string drive;
    bool isTargetLocal;
    std::string sshUser;
    std::string sshHost;
};

#endif // LINUXFREESPACECHECKJOB_H

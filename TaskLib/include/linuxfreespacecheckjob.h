#ifndef LINUXFREESPACECHECKJOB_H
#define LINUXFREESPACECHECKJOB_H

#include "abstractjob.h"

#include "consolejob.h"

class LinuxFreeSpaceCheckJob : public AbstractJob
{
public:
    LinuxFreeSpaceCheckJob();
    LinuxFreeSpaceCheckJob(const std::wstring& _drive);

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    bool IsTargetLocal() const;
    void SetTargetToLocal(const bool value);

    std::wstring GetDrive() const;
    void SetDrive(const std::wstring& value);


private:
    bool IsRemoteTargetConsistent() const;
    AbstractConsoleJob *CreateJobInstance() const;

    std::wstring drive;
    bool isTargetLocal;
    std::wstring sshUser;
    std::wstring sshHost;
};

#endif // LINUXFREESPACECHECKJOB_H

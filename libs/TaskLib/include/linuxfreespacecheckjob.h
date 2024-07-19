#ifndef LINUXFREESPACECHECKJOB_H
#define LINUXFREESPACECHECKJOB_H

#include "diskrelatedjob.h"

class LinuxFreeSpaceCheckJob : public DiskRelatedJob
{
public:
    LinuxFreeSpaceCheckJob(const std::wstring& _drive = L"");
    LinuxFreeSpaceCheckJob(const LinuxFreeSpaceCheckJob& other);

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

    bool IsTargetLocal() const;
    void SetTargetToLocal(const bool value);

private:
    virtual AbstractConsoleJob *CreateJobInstance() const;
    virtual AbstractOutputParser* CreateOutputParser(const int jobStatus) const;

    bool IsRemoteTargetConsistent() const;

    bool isTargetLocal;
    std::wstring sshUser;
    std::wstring sshHost;
};

#endif // LINUXFREESPACECHECKJOB_H

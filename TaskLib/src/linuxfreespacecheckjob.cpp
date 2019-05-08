#include "linuxfreespacecheckjob.h"

#include "consolejob.h"
#include "dfcommandparser.h"
#include "sshconsolejob.h"

using namespace std;

LinuxFreeSpaceCheckJob::LinuxFreeSpaceCheckJob(const std::wstring& _drive)
    : DiskRelatedJob(L"Free space checking", _drive),
      isTargetLocal(true), sshUser(L""), sshHost(L"")
{
}

LinuxFreeSpaceCheckJob::LinuxFreeSpaceCheckJob(const LinuxFreeSpaceCheckJob& other)
   : DiskRelatedJob(other),
     isTargetLocal(other.isTargetLocal),
     sshUser(other.sshUser), sshHost(other.sshHost)
{
}

AbstractJob* LinuxFreeSpaceCheckJob::Clone()
{
   return new LinuxFreeSpaceCheckJob(*this);
}

bool LinuxFreeSpaceCheckJob::InitializeFromClient(Client *client)
{
    if (DiskRelatedJob::InitializeFromClient(client))
    {
        if (isTargetLocal == false)
        {
            if (sshUser == L"")
                sshUser = client->GetProperty(L"sshuser");
            if (sshHost == L"")
                sshHost = client->GetProperty(L"ip");
            return IsRemoteTargetConsistent();
        }
        else
            return true;
    }
    else
        return false;
}

bool LinuxFreeSpaceCheckJob::IsInitialized()
{
   return (isTargetLocal) ? true : IsRemoteTargetConsistent();
}

bool LinuxFreeSpaceCheckJob::IsTargetLocal() const
{
    return isTargetLocal;
}

void LinuxFreeSpaceCheckJob::SetTargetToLocal(const bool value)
{
    isTargetLocal = value;
}

bool LinuxFreeSpaceCheckJob::IsRemoteTargetConsistent() const
{
    return (sshUser != L"" && sshHost != L"");
}

AbstractConsoleJob* LinuxFreeSpaceCheckJob::CreateJobInstance() const
{
    const wstring parameters = wstring(L"-h ") + drive;
    ConsoleJob* dfJob = new ConsoleJob(L"df", parameters);
    if (isTargetLocal)
        return dfJob;
    else
    {
        SshConsoleJob* sshJob = new SshConsoleJob(dfJob);
        sshJob->SetTarget(sshUser, sshHost);
        return sshJob;
    }
}

AbstractOutputParser* LinuxFreeSpaceCheckJob::CreateOutputParser() const
{
   return new DfCommandParser();
}


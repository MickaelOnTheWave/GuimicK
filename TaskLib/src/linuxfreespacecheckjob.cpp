#include "linuxfreespacecheckjob.h"

#include "dfcommandparser.h"
#include "sshconsolejob.h"

using namespace std;

LinuxFreeSpaceCheckJob::LinuxFreeSpaceCheckJob()
    : AbstractJob(L"Free space checking"),
      drive(L""), isTargetLocal(true), sshUser(L""), sshHost(L"")
{
}

LinuxFreeSpaceCheckJob::LinuxFreeSpaceCheckJob(const std::wstring& _drive)
    : AbstractJob(L"Free space checking"),
      drive(_drive), isTargetLocal(true), sshUser(L""), sshHost(L"")
{
}

AbstractJob *LinuxFreeSpaceCheckJob::Clone()
{
    LinuxFreeSpaceCheckJob* job = new LinuxFreeSpaceCheckJob(drive);
    job->isTargetLocal = isTargetLocal;
    job->sshUser = sshUser;
    job->sshHost = sshHost;
    return job;
}

bool LinuxFreeSpaceCheckJob::InitializeFromClient(Client *client)
{
    if (AbstractJob::InitializeFromClient(client))
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

JobStatus *LinuxFreeSpaceCheckJob::Run()
{
    AbstractConsoleJob* job = CreateJobInstance();

    debugManager->AddDataLine<wstring>(L"Command", job->GetCommand());
    debugManager->AddDataLine<wstring>(L"Parameters", job->GetCommandParameters());

    JobStatus* status = job->Run();
    if (status->GetCode() == JobStatus::Ok)
    {
        DfCommandParser parser;
        wstring reportContent = L"";
        bool ok = parser.ParseBuffer(job->GetCommandOutput());
        if (ok)
        {
            status->SetDescription(parser.GetMiniDescription());
            reportContent = parser.GetFullDescription();
        }
        else
        {
            status->SetCode(JobStatus::OkWithWarnings);
            status->SetDescription(L"Failed to parse output");
            reportContent = job->GetCommandOutput();
        }

        if (reportContent != L"")
            status->AddFileBuffer(GetAttachmentName(), reportContent);
    }

    delete job;
    return debugManager->UpdateStatus(status);
}

bool LinuxFreeSpaceCheckJob::IsTargetLocal() const
{
    return isTargetLocal;
}

void LinuxFreeSpaceCheckJob::SetTargetToLocal(const bool value)
{
    isTargetLocal = value;
}

wstring LinuxFreeSpaceCheckJob::GetDrive() const
{
    return drive;
}

void LinuxFreeSpaceCheckJob::SetDrive(const wstring &value)
{
    drive = value;
}

bool LinuxFreeSpaceCheckJob::IsRemoteTargetConsistent() const
{
    return (sshUser != L"" && sshHost != L"");
}

AbstractConsoleJob *LinuxFreeSpaceCheckJob::CreateJobInstance() const
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


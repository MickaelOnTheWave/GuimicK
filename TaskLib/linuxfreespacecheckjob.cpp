#include "linuxfreespacecheckjob.h"

#include "dfcommandparser.h"
#include "sshconsolejob.h"

using namespace std;

LinuxFreeSpaceCheckJob::LinuxFreeSpaceCheckJob()
    : drive(""), isTargetLocal(true), sshUser(""), sshHost("")
{
}

LinuxFreeSpaceCheckJob::LinuxFreeSpaceCheckJob(const std::string &_drive)
    : drive(_drive), isTargetLocal(true), sshUser(""), sshHost("")
{
}

std::string LinuxFreeSpaceCheckJob::GetName()
{
    return "Free space checking";
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
            if (sshUser == "")
                sshUser = client->GetProperty("sshuser");
            if (sshHost == "")
                sshHost = client->GetProperty("ip");
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

    JobStatus* status = job->Run();
    if (status->GetCode() == JobStatus::OK)
    {
        DfCommandParser parser;
        string reportContent = "";
        bool ok = parser.ParseBuffer(job->GetCommandOutput());
        if (ok)
        {
            status->SetDescription(parser.GetMiniDescription());
            reportContent = parser.GetFullDescription();
        }
        else
        {
            status->SetCode(JobStatus::OK_WITH_WARNINGS);
            status->SetDescription("Failed to parse output");
            reportContent = job->GetCommandOutput();
        }

        if (reportContent != "")
            status->AddFileBuffer("FreeSpaceCheck.txt", reportContent);
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

string LinuxFreeSpaceCheckJob::GetDrive() const
{
    return drive;
}

void LinuxFreeSpaceCheckJob::SetDrive(const string &value)
{
    drive = value;
}

bool LinuxFreeSpaceCheckJob::IsRemoteTargetConsistent() const
{
    return (sshUser != "" && sshHost != "");
}

AbstractConsoleJob *LinuxFreeSpaceCheckJob::CreateJobInstance() const
{
    ConsoleJob* dfJob = new ConsoleJob(string("df -h ") + drive);
    if (isTargetLocal)
        return dfJob;
    else
    {
        SshConsoleJob* sshJob = new SshConsoleJob("", dfJob);
        sshJob->SetTarget(sshUser, sshHost);
        return sshJob;
    }
}


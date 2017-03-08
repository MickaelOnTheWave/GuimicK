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

bool LinuxFreeSpaceCheckJob::IsInitialized()
{
    return (isTargetLocal) ? true : IsRemoteTargetConsistent();
}

JobStatus *LinuxFreeSpaceCheckJob::Run()
{
    ConsoleJob* job = CreateJobInstance();

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
    return status;
}

void LinuxFreeSpaceCheckJob::SetTargetToRemote(const bool value)
{
    isTargetLocal = value;
}

bool LinuxFreeSpaceCheckJob::IsRemoteTargetConsistent() const
{
    return (sshUser != "" && sshHost != "");
}

ConsoleJob *LinuxFreeSpaceCheckJob::CreateJobInstance() const
{
    const string dfCommand = string("df -h ") + drive;
    if (isTargetLocal)
        return new ConsoleJob("", dfCommand);
    else
    {
        SshConsoleJob* job = new SshConsoleJob("", dfCommand);
        job->SetTarget(sshUser, sshHost);
        return job;
    }
}


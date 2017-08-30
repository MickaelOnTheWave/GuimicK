#include "sshconsolejob.h"

#include <sstream>
#include "tools.h"

using namespace std;

const string noTargetError = "No target specified";
const string invalidTargetError = "Invalid target specified";

SshConsoleJob::SshConsoleJob(const string& _title, ConsoleJob *_job)
    : title(_title), user(""), host("")
{
    remoteJob = _job;
}

SshConsoleJob::SshConsoleJob(const string &_title, const string &_command)
    : title(_title), user(""), host("")
{
    remoteJob = new ConsoleJob(_command);
}

SshConsoleJob::SshConsoleJob(const SshConsoleJob &other)
{
    title = other.title;
    user = other.user;
    host = other.host;
    remoteJob = static_cast<ConsoleJob*>(other.remoteJob->Clone());
}

SshConsoleJob::~SshConsoleJob()
{
    delete remoteJob;
}

string SshConsoleJob::GetName()
{
    return title;
}

AbstractJob *SshConsoleJob::Clone()
{
    return new SshConsoleJob(*this);
}

void SshConsoleJob::SetTarget(const string &_user, const string &_host)
{
    user = _user;
    host= _host;
}

bool SshConsoleJob::InitializeFromClient(Client *client)
{
    if (AbstractConsoleJob::InitializeFromClient(client))
    {
        if (user == "")
            user = client->GetProperty("sshuser");

        if (host == "")
            host = client->GetProperty("ip");

        return IsInitialized();
    }
    else
        return false;
}

bool SshConsoleJob::IsInitialized()
{
    return (user != "" && host != "");
}

JobStatus *SshConsoleJob::Run()
{
    if (IsInitialized() == false)
        return new JobStatus(JobStatus::ERROR, noTargetError);
    else if (Tools::IsComputerAlive(host) == false)
        return new JobStatus(JobStatus::ERROR, invalidTargetError);

    string oldCommandName = remoteJob->GetCommand();
    string newCommandName = string("ssh ") + user + "@" + host + " \"" + oldCommandName + "\"";
    remoteJob->SetCommand(newCommandName);

    debugManager->AddDataLine<string>("Full command", newCommandName);

    JobStatus* status = remoteJob->Run();

    debugManager->AddDataLine<string>("Output", remoteJob->GetCommandOutput());
    debugManager->AddDataLine<int>("Return code", remoteJob->GetCommandReturnCode());

    remoteJob->SetCommand(oldCommandName);
    return debugManager->UpdateStatus(status);
}

int SshConsoleJob::GetExpectedReturnCode() const
{
    return remoteJob->GetExpectedReturnCode();
}

void SshConsoleJob::SetExpectedReturnCode(const int value)
{
    remoteJob->SetExpectedReturnCode(value);
}

string SshConsoleJob::GetCommand() const
{
    return remoteJob->GetCommand();
}

void SshConsoleJob::SetCommand(const string &command)
{
    remoteJob->SetCommand(command);
}

int SshConsoleJob::GetCommandReturnCode() const
{
    return remoteJob->GetCommandReturnCode();
}

string SshConsoleJob::GetCommandOutput() const
{
    return remoteJob->GetCommandOutput();
}


#include "sshconsolejob.h"

#include <sstream>
#include "consolejob.h"
#include "tools.h"
#include "userconsolejob.h"

using namespace std;

const string noTargetError = "No target specified";
const string invalidTargetError = "Invalid target specified";

SshConsoleJob::SshConsoleJob(AbstractConsoleJob *_job)
    : AbstractConsoleJob(), title(""), user(""), host("")
{
    SetRemoteJob(_job);
}

SshConsoleJob::SshConsoleJob(const string &_title, const string &_command)
    : AbstractConsoleJob(), title(_title), user(""), host("")
{

    SetRemoteJob(new ConsoleJob(_command));
}

SshConsoleJob::SshConsoleJob(const SshConsoleJob &other)
    : AbstractConsoleJob(other)
{
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
    return remoteJob->GetName();
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

    AbstractConsoleJob* sshJob = CreateSshJob();

    debugManager->AddDataLine<string>("Child command", sshJob->GetCommand());
    debugManager->AddDataLine<string>("Child params", sshJob->GetCommandParameters());

    JobStatus* status = sshJob->Run();

    debugManager->AddDataLine<string>("Output", sshJob->GetCommandOutput());
    debugManager->AddDataLine<int>("Return code", sshJob->GetCommandReturnCode());

    remoteJob->SetCommandReturnCode(sshJob->GetCommandReturnCode());
    remoteJob->SetCommandOutput(sshJob->GetCommandOutput());

    delete sshJob;
    return debugManager->UpdateStatus(status);
}

void SshConsoleJob::SetTitle(const string &value)
{
    title = value;
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

string SshConsoleJob::GetCommandParameters() const
{
    return remoteJob->GetCommandParameters();
}

void SshConsoleJob::SetCommandParameters(const string &parameters)
{
    remoteJob->SetCommandParameters(parameters);
}

int SshConsoleJob::GetCommandReturnCode() const
{
    return remoteJob->GetCommandReturnCode();
}

void SshConsoleJob::SetCommandReturnCode(const int value)
{
    remoteJob->SetCommandReturnCode(value);
}

string SshConsoleJob::GetCommandOutput() const
{
    return remoteJob->GetCommandOutput();
}

void SshConsoleJob::SetCommandOutput(const string &value)
{
    remoteJob->SetCommandOutput(value);
}

bool SshConsoleJob::IsCommandAvailable() const
{
    // No way to check remotely if command exists.
    return true;
}

bool SshConsoleJob::IsRunOk() const
{
    return remoteJob->IsRunOk();
}

void SshConsoleJob::SetRemoteJob(AbstractConsoleJob *_remoteJob)
{
    remoteJob = _remoteJob;
    remoteJob->SetParentDebugManager(debugManager);
}

string SshConsoleJob::GetExpectedOutput() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->GetExpectedOutput() : string("");
}

void SshConsoleJob::SetExpectedOutput(const string &value)
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    if (castJob)
        castJob->SetExpectedOutput(value);
}

string SshConsoleJob::GetOutputFile() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->GetOutputFile() : string("");
}

string SshConsoleJob::GetMiniDescriptionParserCommand() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->GetMiniDescriptionParserCommand() : string("");
}

bool SshConsoleJob::IsParsingUsingBuffer() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->IsParsingUsingBuffer() : false;
}

AbstractConsoleJob *SshConsoleJob::CreateSshJob()
{
    AbstractConsoleJob* sshJob = static_cast<AbstractConsoleJob*>(remoteJob->Clone());
    const string remoteJobCommand = remoteJob->GetCommand() + " " +
                                    remoteJob->GetCommandParameters();
    const string sshParameters = user + "@" + host + " \"" + remoteJobCommand + "\"";
    sshJob->SetCommand("ssh");
    sshJob->SetCommandParameters(sshParameters);
    sshJob->SetParentDebugManager(debugManager);

    debugManager->AddDataLine<string>("Ssh parameters", sshParameters);
    return sshJob;
}


#include "sshconsolejob.h"

#include <sstream>

using namespace std;

SshConsoleJob::SshConsoleJob(const std::string& _commandTitle, const std::string& _command,
                             int _expectedReturnCode)
    : UserConsoleJob(_commandTitle, _command, _expectedReturnCode),
      user(""), host("")
{
}

SshConsoleJob::SshConsoleJob(const SshConsoleJob &other)
    : UserConsoleJob(other)
{
    user = other.user;
    host = other.host;
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
    if (user == "")
        user = client->GetProperty("sshuser");

    if (host == "")
        host = client->GetProperty("ip");

    return IsInitialized();
}

bool SshConsoleJob::IsInitialized()
{
    return (user != "" && host != "");
}

JobStatus *SshConsoleJob::Run()
{
    string remoteCommand = command;
    command = string("ssh ") + user + "@" + host + " \"" + remoteCommand + "\"";

    JobStatus* status = ConsoleJob::Run();

    command = remoteCommand;
    return status;
}


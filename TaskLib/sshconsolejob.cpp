#include "sshconsolejob.h"

#include <sstream>

using namespace std;

SshConsoleJob::SshConsoleJob(const std::string& _commandTitle, const std::string& _command,
                             int _expectedReturnCode)
    : ConsoleJob(_commandTitle, _command, _expectedReturnCode),
      user(""), host("")
{
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

    return (user != "" && host != "");
}

bool SshConsoleJob::IsInitialized()
{
    return (user != "" && host != "");
}

JobStatus *SshConsoleJob::Run()
{
    return ConsoleJob::Run();
}

string SshConsoleJob::CreateFullCommand()
{
    string localCommand = ConsoleJob::CreateFullCommand();
    return string("ssh ") + user + "@" + host + " \"" + localCommand + "\"";
}


#include "consolejob.h"

#include "jobstatus.h"
#include <stdlib.h>
#include <tools.h>

using namespace std;

static string whichCommandPath("");
std::vector<std::string> ConsoleJob::appSearchPaths;

ConsoleJob::ConsoleJob(const string &_command,
                       const string &_params, int _expectedReturnCode)
    : command(_command), commandParams(_params), commandOutput(""),
      expectedReturnCode(_expectedReturnCode), receivedReturnCode(-1)
{
    string foundCommandFullName = Tools::GetCommandPath(command, appSearchPaths);
    if (foundCommandFullName != "")
        command = foundCommandFullName;
}

ConsoleJob::ConsoleJob(const ConsoleJob &other)
{
    command = other.command;
    commandParams = other.commandParams;
    commandOutput = other.commandOutput;
    expectedReturnCode = other.expectedReturnCode;
    receivedReturnCode = other.receivedReturnCode;
}

ConsoleJob::~ConsoleJob()
{
}

std::string ConsoleJob::GetName()
{
    return "";
}

AbstractJob *ConsoleJob::Clone()
{
    return new ConsoleJob(*this);
}

bool ConsoleJob::InitializeFromClient(Client *)
{
    return true;
}

bool ConsoleJob::IsInitialized()
{
    return (command != "");
}

JobStatus *ConsoleJob::Run()
{
    bool success = RunCommand();
    if (success)
        return CreateSuccessStatus();
    else
        return CreateErrorStatus();
}

int ConsoleJob::GetExpectedReturnCode() const
{
    return expectedReturnCode;
}

void ConsoleJob::SetExpectedReturnCode(const int value)
{
    expectedReturnCode = value;
}

string ConsoleJob::GetCommand() const
{
    return command;
}

void ConsoleJob::SetCommand(const string &_command)
{
    command = _command;
}

int ConsoleJob::GetCommandReturnCode()
{
    return receivedReturnCode;
}

string ConsoleJob::GetCommandOutput() const
{
    return commandOutput;
}

void ConsoleJob::AddAppSearchPath(const string &path)
{
    appSearchPaths.push_back(path);
}

void ConsoleJob::ClearAppSearchPaths()
{
    appSearchPaths.clear();
}

bool ConsoleJob::RunCommand()
{
    string commandToRun = command + " " + commandParams;
    int rawCode = Tools::RunExternalCommandToBuffer(commandToRun, commandOutput, true);
    receivedReturnCode = WEXITSTATUS(rawCode);
    return (receivedReturnCode == expectedReturnCode);
}

JobStatus *ConsoleJob::CreateSuccessStatus()
{
    return new JobStatus(JobStatus::OK, "");
}

JobStatus *ConsoleJob::CreateErrorStatus()
{
    return new JobStatus(JobStatus::ERROR, "");
}

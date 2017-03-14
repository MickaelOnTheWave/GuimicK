#include "consolejob.h"

#include "jobstatus.h"
#include <stdlib.h>
#include <tools.h>

using namespace std;

static string whichCommandPath("");
std::vector<std::string> ConsoleJob::appSearchPaths;

ConsoleJob::ConsoleJob(const string &_command, int _expectedReturnCode)
    : command(_command), commandOutput(""),
      expectedReturnCode(_expectedReturnCode), receivedReturnCode(-1)
{
    string foundCommandFullName = Tools::GetCommandPath(command, appSearchPaths);
    if (foundCommandFullName != "")
        command = foundCommandFullName;
}

ConsoleJob::ConsoleJob(const ConsoleJob &other)
{
    command = other.command;
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
    int rawCode = Tools::RunExternalCommandToBuffer(command, commandOutput, true);
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

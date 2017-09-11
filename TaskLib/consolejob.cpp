#include "consolejob.h"

#include "jobstatus.h"
#include <stdlib.h>
#include <tools.h>

using namespace std;

static string whichCommandPath("");
string ConsoleJob::NotAvailableError = "Command not available";

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

bool ConsoleJob::IsInitialized()
{
    return (command != "");
}

JobStatus *ConsoleJob::Run()
{
    if (IsCommandAvailable() == false)
        return debugManager->CreateStatus(JobStatus::ERROR, NotAvailableError);

    bool success = RunCommand();
    if (success)
        return CreateSuccessStatus();
    else
        return CreateErrorStatus();
}

void ConsoleJob::Run(const string &command, const string &params)
{
    ConsoleJob job(command, params);
    job.RunCommand();
}

void ConsoleJob::RunWithoutStatus()
{
    RunCommand();
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

void ConsoleJob::SetCommandParameters(const string &parameters)
{
    commandParams = parameters;
}

string ConsoleJob::GetCommandParameters() const
{
    return commandParams;
}

int ConsoleJob::GetCommandReturnCode() const
{
    return receivedReturnCode;
}

void ConsoleJob::SetCommandReturnCode(const int value)
{
    receivedReturnCode = value;
}

string ConsoleJob::GetCommandOutput() const
{
    return commandOutput;
}

void ConsoleJob::SetCommandOutput(const string &value)
{
    commandOutput = value;
}

bool ConsoleJob::IsRunOk() const
{
    return receivedReturnCode == expectedReturnCode;
}

bool ConsoleJob::IsCommandAvailable() const
{
    return (Tools::GetCommandPath(command, appSearchPaths) != string(""));
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

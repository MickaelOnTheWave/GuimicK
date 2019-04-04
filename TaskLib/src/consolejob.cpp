#include "consolejob.h"

#include "jobstatus.h"
#include <pathtools.h>
#include <stdlib.h>
#include <tools.h>

using namespace std;

static wstring whichCommandPath = L"";
wstring ConsoleJob::NotAvailableError = L"Command not available";

vector<wstring> ConsoleJob::appSearchPaths;

ConsoleJob::ConsoleJob(const wstring &_command,
                       const wstring &_params, int _expectedReturnCode)
    : AbstractConsoleJob(L""),
      command(_command), commandParams(_params), commandOutput(L""),
      expectedReturnCode(_expectedReturnCode), receivedReturnCode(-1)
{
    wstring foundCommandFullName = PathTools::GetCommandPath(command, appSearchPaths);
    if (foundCommandFullName != L"")
        command = foundCommandFullName;
}

ConsoleJob::ConsoleJob(const ConsoleJob &other)
   : AbstractConsoleJob(other)
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

AbstractJob *ConsoleJob::Clone()
{
    return new ConsoleJob(*this);
}

bool ConsoleJob::IsInitialized()
{
    return (command != L"");
}

JobStatus *ConsoleJob::Run()
{
    bool success = RunCommand();
    if (success)
        return CreateSuccessStatus();
    else
        return CreateErrorStatus();
}

void ConsoleJob::Run(const wstring &command, const wstring &params)
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

wstring ConsoleJob::GetCommand() const
{
    return command;
}

void ConsoleJob::SetCommand(const wstring &_command)
{
    command = _command;
}

void ConsoleJob::SetCommandParameters(const wstring &parameters)
{
    commandParams = parameters;
}

wstring ConsoleJob::GetCommandParameters() const
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

wstring ConsoleJob::GetCommandOutput() const
{
    return commandOutput;
}

void ConsoleJob::SetCommandOutput(const wstring &value)
{
    commandOutput = value;
}

bool ConsoleJob::IsRunOk() const
{
    return receivedReturnCode == expectedReturnCode;
}

bool ConsoleJob::IsCommandAvailable() const
{
#ifdef _WIN32
   return true;
#else
   return (PathTools::GetCommandPath(command, appSearchPaths) != wstring(L""));
#endif
}

void ConsoleJob::AddAppSearchPath(const wstring &path)
{
    appSearchPaths.push_back(path);
}

void ConsoleJob::ClearAppSearchPaths()
{
    appSearchPaths.clear();
}

bool ConsoleJob::RunCommand()
{
    wstring commandToRun = command + L" " + commandParams;
    int rawCode = Tools::RunExternalCommandToBuffer(commandToRun, commandOutput, true);
#ifdef __linux__
    receivedReturnCode = WEXITSTATUS(rawCode);
#elif _WIN32
    receivedReturnCode = rawCode;
#endif
    return (receivedReturnCode == expectedReturnCode);
}

JobStatus *ConsoleJob::CreateSuccessStatus()
{
    return new JobStatus(JobStatus::Ok, L"");
}

JobStatus *ConsoleJob::CreateErrorStatus()
{
    return new JobStatus(JobStatus::Error, L"");
}

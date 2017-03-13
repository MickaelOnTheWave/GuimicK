#include "userconsolejob.h"

#include "tools.h"
#include <stdlib.h>
#include <sstream>

using namespace std;

UserConsoleJob::UserConsoleJob()
    : ConsoleJob(), debugInfo("", false)
{
}

UserConsoleJob::UserConsoleJob(const std::string &_commandTitle,
                               const std::string &_command, int _expectedReturnCode)
    : ConsoleJob(_command, _expectedReturnCode), commandTitle(_commandTitle),
      debugInfo(_commandTitle, false)
{
    Initialize(_command, _expectedReturnCode);
}

UserConsoleJob::UserConsoleJob(const UserConsoleJob &other)
    : ConsoleJob(other), debugInfo(other.debugInfo)
{
    commandTitle = other.commandTitle;
    attachOutputToStatus = other.attachOutputToStatus;
    parserCommand = other.parserCommand;
    checkReturnCode = other.checkReturnCode;
    checkStandardOutput = other.checkStandardOutput;
    outputFileName = other.outputFileName;
    expectedOutput = other.expectedOutput;
}

UserConsoleJob::~UserConsoleJob()
{

}

std::string UserConsoleJob::GetName()
{
    return commandTitle;
}

AbstractJob *UserConsoleJob::Clone()
{
    return new UserConsoleJob(*this);
}

void UserConsoleJob::Initialize(const string &_command, int _expectedReturnCode)
{
    currentStatus = NULL;
    command = _command;
    outputFileName = "";
    expectedReturnCode = _expectedReturnCode;
    receivedReturnCode = -1;
    commandOutput = "";
    expectedOutput = "";
    checkReturnCode = true;
    checkStandardOutput = false;
    attachOutputToStatus = false;

    IsInitialized();
}

void UserConsoleJob::SetTitle(const std::string &title)
{
    commandTitle = title;
}

void UserConsoleJob::SetMiniDescriptionParserCommand(const string &parser)
{
    parserCommand = parser;
}

void UserConsoleJob::SetAttachOutput(const bool value)
{
    attachOutputToStatus = value;
}

void UserConsoleJob::SetOutputDebugInformation(const bool value)
{
    debugInfo.SetUse(value);
}

void UserConsoleJob::SetOutputTofile(const string &filename)
{
    outputFileName = filename;
    expectedOutput = "";
    checkStandardOutput = false;
}

void UserConsoleJob::SetOutputToBuffer()
{
    outputFileName = "";
}

void UserConsoleJob::SetExpectedReturnCode(const int value)
{
    ConsoleJob::SetExpectedReturnCode(value);

    expectedOutput = "";
    checkReturnCode = true;
    checkStandardOutput = false;
}
/*
void UserConsoleJob::EnableSuccessOnReturnCode(int code)
{
    expectedReturnCode = code;
    checkReturnCode = true;
}

void UserConsoleJob::DisableSuccessOnReturnCode()
{
    expectedReturnCode = -1;
    checkReturnCode = false;
}

void UserConsoleJob::EnableSuccessOnOutput(const string &output)
{
    outputFileName = "";
    expectedOutput = output;
    checkStandardOutput = true;
}

void UserConsoleJob::DisableSuccessOnOutput()
{
    expectedOutput = "";
    checkStandardOutput = false;
}
*/
string UserConsoleJob::GetExpectedOutput() const
{
    return expectedOutput;
}

void UserConsoleJob::SetExpectedOutput(const string &value)
{
    expectedOutput = value;

    expectedReturnCode = -1;
    checkStandardOutput = true;
    checkReturnCode = false;
}

// @TODO resolve bug where apparently output is not correctly processed with it has some special chars (like in :-) )
bool UserConsoleJob::RunCommand()
{
    debugInfo.Reset();

    debugInfo.AddStringDataLine("Command", command);

    delete currentStatus;
    currentStatus = new JobStatus();
    if (outputFileName != "")
        RunCommandOnFile();
    else
        RunCommandOnBuffer();

    debugInfo.AddIntDataLine("Received return code", receivedReturnCode);

    return IsRunOk();
}

JobStatus *UserConsoleJob::CreateSuccessStatus()
{
    if (parserCommand != "")
        FillStatusFromParsing();
    else
        currentStatus->SetCode(JobStatus::OK);

    FinalizeStatusCreation();
    return currentStatus;
}

JobStatus *UserConsoleJob::CreateErrorStatus()
{
    if (IsCheckingOutput())
        FillErrorStatusFromOutput();
    else
        FillErrorStatusFromReturnCode();

    FinalizeStatusCreation();
    return currentStatus;
}

void UserConsoleJob::RunCommandOnFile()
{
    int rawCode = Tools::RunExternalCommandToFile(command, outputFileName, true);
    receivedReturnCode = WEXITSTATUS(rawCode);
    currentStatus->AddExternalFile(outputFileName);
}

void UserConsoleJob::RunCommandOnBuffer()
{
    ConsoleJob::RunCommand();
    if (attachOutputToStatus)
        currentStatus->AddFileBuffer(commandTitle + ".txt", commandOutput);
}

bool UserConsoleJob::IsRunOk()
{
    if (IsCheckingOutput())
        return commandOutput == expectedOutput;
    else
        return receivedReturnCode == expectedReturnCode;
}

bool UserConsoleJob::IsCheckingOutput()
{
    return checkStandardOutput;
}

void UserConsoleJob::FillStatusFromParsing()
{
    debugInfo.AddStringDataLine("Parser command", parserCommand);

    // TODO make parserCommand smarter! To use directly output from command.
    string miniDescription("");
    int returnValue = Tools::RunExternalCommandToBuffer(parserCommand, miniDescription);
    if (returnValue != -1)
    {
        currentStatus->SetCode(JobStatus::OK);
        currentStatus->SetDescription(miniDescription);
    }
    else
    {
        currentStatus->SetCode(JobStatus::OK_WITH_WARNINGS);
        currentStatus->SetDescription("Parsing error");
    }
}

void UserConsoleJob::FillErrorStatusFromOutput()
{
    currentStatus->SetCode(JobStatus::ERROR);

    stringstream message;
    message << "Received <" << commandOutput << "> - expected <" << expectedOutput << ">" << endl;
    currentStatus->SetDescription(message.str());
}

void UserConsoleJob::FillErrorStatusFromReturnCode()
{
    currentStatus->SetCode(JobStatus::ERROR);

    stringstream message;
    message << "Return value : " << receivedReturnCode << " - expected : " << expectedReturnCode << endl;
    currentStatus->SetDescription(message.str());
}

void UserConsoleJob::FinalizeStatusCreation()
{
    currentStatus = debugInfo.UpdateStatus(currentStatus);
}

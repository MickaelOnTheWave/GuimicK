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
                               const std::string &_command, const std::string& _params,
                               int _expectedReturnCode)
    : ConsoleJob(_command, _params, _expectedReturnCode), commandTitle(_commandTitle),
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
    successConditionOnStandardOutput = other.successConditionOnStandardOutput;
    outputFileName = other.outputFileName;
    expectedOutput = other.expectedOutput;
    useParserWithBuffer = other.useParserWithBuffer;

    if (other.currentStatus)
        currentStatus = new JobStatus(*other.currentStatus);
    else
        currentStatus = NULL;
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
    successConditionOnStandardOutput = false;
    attachOutputToStatus = false;
    useParserWithBuffer = false;

    IsInitialized();
}

void UserConsoleJob::SetTitle(const std::string &title)
{
    commandTitle = title;
}

void UserConsoleJob::SetMiniDescriptionParserCommand(const string &parser)
{
    parserCommand = parser;
    attachOutputToStatus = false;
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
    successConditionOnStandardOutput = false;
}

void UserConsoleJob::SetOutputToBuffer()
{
    outputFileName = "";
}

void UserConsoleJob::SetParsingUsingBuffer(const bool value)
{
    useParserWithBuffer = value;
}

void UserConsoleJob::SetExpectedReturnCode(const int value)
{
    ConsoleJob::SetExpectedReturnCode(value);

    expectedOutput = "";
    successConditionOnStandardOutput = false;
}

string UserConsoleJob::GetExpectedOutput() const
{
    return expectedOutput;
}

void UserConsoleJob::SetExpectedOutput(const string &value)
{
    expectedOutput = value;

    expectedReturnCode = -1;
    successConditionOnStandardOutput = true;
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
    if (successConditionOnStandardOutput)
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
    if (attachOutputToStatus && commandOutput != "")
        currentStatus->AddFileBuffer(commandTitle + ".txt", commandOutput);
}

bool UserConsoleJob::IsRunOk()
{
    if (successConditionOnStandardOutput)
        return commandOutput == expectedOutput;
    else
        return receivedReturnCode == expectedReturnCode;
}

void UserConsoleJob::FillStatusFromParsing()
{
    debugInfo.AddStringDataLine("Parser command", parserCommand);

    string miniDescription("");
    int returnValue = Tools::RunExternalCommandToBuffer(CreateParserCommand(), miniDescription, true);
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

string UserConsoleJob::CreateParserCommand() const
{
    if (useParserWithBuffer)
        return parserCommand + " \"" + commandOutput + "\"";
    else
        return parserCommand;
}

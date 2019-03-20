#include "userconsolejob.h"

#include "tools.h"
#include <stdlib.h>
#include <sstream>

using namespace std;

UserConsoleJob::UserConsoleJob()
    : ConsoleJob()
{
   SetName(L"Console");
   Initialize(L"", 0);
}

UserConsoleJob::UserConsoleJob(const std::wstring &_commandTitle,
                               const std::wstring &_command, const std::wstring& _params,
                               int _expectedReturnCode)
    : ConsoleJob(_command, _params, _expectedReturnCode)
{
   SetName(_commandTitle != L"" ? _commandTitle : L"Console");
   Initialize(_command, _expectedReturnCode);
}

UserConsoleJob::UserConsoleJob(const UserConsoleJob &other)
    : ConsoleJob(other)
{
    attachOutputToStatus = other.attachOutputToStatus;
    parserCommand = other.parserCommand;
    successConditionOnStandardOutput = other.successConditionOnStandardOutput;
    outputFileName = other.outputFileName;
    expectedOutput = other.expectedOutput;
    useParserWithBuffer = other.useParserWithBuffer;
    additionalAttachments = other.additionalAttachments;

    if (other.currentStatus)
        currentStatus = new JobStatus(*other.currentStatus);
    else
        currentStatus = NULL;
}

UserConsoleJob::~UserConsoleJob()
{

}

AbstractJob *UserConsoleJob::Clone()
{
    return new UserConsoleJob(*this);
}

void UserConsoleJob::Initialize(const wstring &_command, int _expectedReturnCode)
{
    currentStatus = NULL;
    command = _command;
    outputFileName = L"";
    expectedReturnCode = _expectedReturnCode;
    receivedReturnCode = -1;
    commandOutput = L"";
    expectedOutput = L"";
    successConditionOnStandardOutput = false;
    attachOutputToStatus = false;
    useParserWithBuffer = false;

    IsInitialized();
}

JobStatus *UserConsoleJob::Run()
{
    if (IsCommandAvailable() == false)
        return debugManager->CreateStatus(JobStatus::Error, NotAvailableError);

    return ConsoleJob::Run();
}

wstring UserConsoleJob::GetMiniDescriptionParserCommand() const
{
    return parserCommand;
}

void UserConsoleJob::SetMiniDescriptionParserCommand(const wstring &parser)
{
    parserCommand = parser;
    attachOutputToStatus = false;
}

void UserConsoleJob::SetAttachOutput(const bool value)
{
    attachOutputToStatus = value;
}

wstring UserConsoleJob::GetOutputFile() const
{
    return outputFileName;
}

void UserConsoleJob::SetOutputTofile(const wstring &filename)
{
    outputFileName = filename;
    expectedOutput = L"";
    successConditionOnStandardOutput = false;
}

void UserConsoleJob::SetOutputToBuffer()
{
    outputFileName = L"";
}

bool UserConsoleJob::IsParsingUsingBuffer() const
{
    return useParserWithBuffer;
}

void UserConsoleJob::SetParsingUsingBuffer(const bool value)
{
    useParserWithBuffer = value;
}

void UserConsoleJob::SetExpectedReturnCode(const int value)
{
    ConsoleJob::SetExpectedReturnCode(value);

    expectedOutput = L"";
    successConditionOnStandardOutput = false;
}

wstring UserConsoleJob::GetExpectedOutput() const
{
    return expectedOutput;
}

void UserConsoleJob::SetExpectedOutput(const wstring &value)
{
    expectedOutput = value;

    expectedReturnCode = -1;
    successConditionOnStandardOutput = true;
}

bool UserConsoleJob::HasUserAttachments() const
{
   return (!additionalAttachments.empty());
}

void UserConsoleJob::GetUserAttachments(vector<wstring>& attachments)
{
   attachments = additionalAttachments;
}

void UserConsoleJob::AddUserAttachment(const wstring& name)
{
   additionalAttachments.push_back(name);
}

void UserConsoleJob::EmptyUserAttachments()
{
   additionalAttachments.clear();
}

// @TODO resolve bug where apparently output is not correctly processed with it has some
// special chars (like in :-) )
bool UserConsoleJob::RunCommand()
{
    debugManager->Reset();

    debugManager->AddDataLine<wstring>(L"Command", command);

    delete currentStatus;
    currentStatus = new JobStatus();
    if (outputFileName != L"")
        RunCommandOnFile();
    else
        RunCommandOnBuffer();

    debugManager->AddDataLine<int>(L"Received return code", receivedReturnCode);

    return IsRunOk();
}

JobStatus *UserConsoleJob::CreateSuccessStatus()
{
    if (parserCommand != L"")
        FillStatusFromParsing();
    else
        currentStatus->SetCode(JobStatus::Ok);

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
    wstring commandToRun = command + L" " + commandParams;
    int rawCode = Tools::RunExternalCommandToFile(commandToRun, outputFileName, true);
#ifdef __linux__
    receivedReturnCode = WEXITSTATUS(rawCode);
#elif _WIN32
    receivedReturnCode = rawCode;
#endif
    currentStatus->AddExternalFile(outputFileName);
}

void UserConsoleJob::RunCommandOnBuffer()
{
    ConsoleJob::RunCommand();
    if (attachOutputToStatus && commandOutput != L"")
        currentStatus->AddFileBuffer(GetAttachmentName(), commandOutput);
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
    debugManager->AddDataLine<wstring>(L"Parser command", parserCommand);

    wstring miniDescription(L"");
    int returnValue = Tools::RunExternalCommandToBuffer(CreateParserCommand(), miniDescription, true);
    if (returnValue != -1)
    {
        currentStatus->SetCode(JobStatus::Ok);
        currentStatus->SetDescription(miniDescription);
    }
    else
    {
        currentStatus->SetCode(JobStatus::OkWithWarnings);
        currentStatus->SetDescription(L"Parsing error");
    }
}

void UserConsoleJob::FillErrorStatusFromOutput()
{
    currentStatus->SetCode(JobStatus::Error);

    wstringstream message;
    message << "Received <" << commandOutput << "> - expected <" << expectedOutput << ">" << endl;
    currentStatus->SetDescription(message.str());
}

void UserConsoleJob::FillErrorStatusFromReturnCode()
{
    currentStatus->SetCode(JobStatus::Error);

    wstringstream message;
    message << "Return value : " << receivedReturnCode << " - expected : " << expectedReturnCode << endl;

    currentStatus->SetDescription(message.str());

    debugManager->AddDataLine<wstring>(L"Output", commandOutput);
}

void UserConsoleJob::FinalizeStatusCreation()
{
    currentStatus = debugManager->UpdateStatus(currentStatus);

    vector<wstring>::const_iterator it = additionalAttachments.begin();
    for (; it != additionalAttachments.end(); ++it)
       currentStatus->AddExternalFile(*it);
}

wstring UserConsoleJob::CreateParserCommand() const
{
    if (useParserWithBuffer)
        return parserCommand + L" \"" + commandOutput + L"\"";
    else
        return parserCommand;
}

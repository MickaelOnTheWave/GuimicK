#include "consolejob.h"

#include "jobstatus.h"
#include <filetools.h>
#include <tools.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

static string whichCommandPath("");

vector<string> ConsoleJob::appSearchPaths;

ConsoleJob::ConsoleJob()
{
}

// @TODO Check how to prevent commands from unwillingly outputting to console (ls error case for example).
ConsoleJob::ConsoleJob(const string& _commandTitle, const string &_commandName,
                       const string &_commandParameters, int _expectedReturnCode)
    : commandTitle(_commandTitle), parserCommand("")
{
    Initialize(_commandName, _commandParameters, _expectedReturnCode);
}

ConsoleJob::~ConsoleJob()
{
}

std::string ConsoleJob::GetName()
{
    return commandTitle;
}

AbstractJob *ConsoleJob::Clone()
{
    ConsoleJob* clone = new ConsoleJob();
    clone->commandTitle = commandTitle;
    clone->commandName = commandName;
    clone->commandParameters = commandParameters;
    clone->parserCommand = parserCommand;
    clone->checkReturnCode = checkReturnCode;
    clone->checkStandardOutput = checkStandardOutput;
    clone->attachOutputToStatus = attachOutputToStatus;
    clone->outputFileName = outputFileName;
    clone->standardOutput = standardOutput;
    clone->expectedOutput = expectedOutput;
    clone->expectedReturnCode = expectedReturnCode;
    clone->receivedReturnCode = receivedReturnCode;
    return clone;
}

bool ConsoleJob::InitializeFromClient(Client *)
{
    return true;
}

void ConsoleJob::SetTitle(const string &_title)
{
    commandTitle = _title;
}

void ConsoleJob::SetMiniDescriptionParserCommand(const string &parser)
{
    parserCommand = parser;
}

void ConsoleJob::Initialize(const string &_commandName, const string &_commandParameters, int _expectedReturnCode)
{
    commandName = _commandName;
    commandParameters = _commandParameters;
    outputFileName = "";
    receivedReturnCode = -1;
    standardOutput = "";
    expectedOutput = "";
    checkStandardOutput = false;
    attachOutputToStatus = false;
    EnableSuccessOnReturnCode(_expectedReturnCode);

    IsInitialized();
}

bool ConsoleJob::IsInitialized()
{
    commandName = Tools::GetCommandPath(commandName, appSearchPaths);
    return (commandName != "");
}

// @TODO resolve bug where apparently output is not correctly processed with it has some special chars (like in :-) )
JobStatus *ConsoleJob::Run()
{
    JobStatus* status = new JobStatus(JobStatus::OK);
    const string fullCommand = CreateFullCommand();

    if (outputFileName != "")
    {
        receivedReturnCode = Tools::RunExternalCommandToFile(fullCommand, outputFileName, true);
        status->AddExternalFile(outputFileName);
    }
    else
    {
        receivedReturnCode = Tools::RunExternalCommandToBuffer(fullCommand, standardOutput, true);
        if (attachOutputToStatus)
            status->AddFileBuffer(commandTitle + ".txt", standardOutput);

    }

    receivedReturnCode = WEXITSTATUS(receivedReturnCode);

    stringstream sstream;
    if (checkReturnCode && receivedReturnCode != expectedReturnCode)
    {
        status->SetCode(JobStatus::ERROR);
        sstream << "Return value : " << receivedReturnCode << " - expected : " << expectedReturnCode << endl;
    }
    if (checkStandardOutput)
    {
        if (standardOutput != expectedOutput)
        {
            status->SetCode(JobStatus::ERROR);
            sstream << "Received <" << standardOutput << "> - expected <" << expectedOutput << ">" << endl;
        }
        else if (outputFileName == "" && status->GetCode() == JobStatus::OK)
            sstream << standardOutput;
    }

    if (parserCommand != "" && status->GetCode() == JobStatus::OK)
    {
        // TODO make parserCommand smarter! To use directly output from command.
        string miniDescription("");
        int returnValue = Tools::RunExternalCommandToBuffer(parserCommand, miniDescription);
        if (returnValue != -1)
            status->SetDescription(miniDescription);
        else
            status->SetDescription("parsing error");
    }
    else
        status->SetDescription(sstream.str());

    return status;
}

int ConsoleJob::GetCommandReturnCode()
{
    return receivedReturnCode;
}

string ConsoleJob::GetCommandOutput() const
{
    return (outputFileName != "") ? outputFileName : standardOutput;
}

void ConsoleJob::SetOutputTofile(const string &filename)
{
    outputFileName = filename;
    expectedOutput = "";
    checkStandardOutput = false;
}

void ConsoleJob::SetOutputToBuffer()
{
    outputFileName = "";
}

void ConsoleJob::AttachOutputToStatus()
{
    attachOutputToStatus = true;
}

void ConsoleJob::EnableSuccessOnReturnCode(int code)
{
    expectedReturnCode = code;
    checkReturnCode = true;
}

void ConsoleJob::DisableSuccessOnReturnCode()
{
    expectedReturnCode = -1;
    checkReturnCode = false;
}

void ConsoleJob::EnableSuccessOnOutput(const string &output)
{
    outputFileName = "";
    expectedOutput = output;
    checkStandardOutput = true;
}

void ConsoleJob::DisableSuccessOnOutput()
{
    expectedOutput = "";
    checkStandardOutput = false;
}

void ConsoleJob::AddAppSearchPath(const string &path)
{
    appSearchPaths.push_back(path);
}

void ConsoleJob::ClearAppSearchPaths()
{
    appSearchPaths.clear();
}

string ConsoleJob::CreateFullCommand()
{
    return commandName + " " + commandParameters;
}


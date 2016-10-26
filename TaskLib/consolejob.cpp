#include "consolejob.h"

#include "jobstatus.h"
#include <filetools.h>
#include <tools.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

static string whichCommandPath("");

vector<string> ConsoleJob::appSearchPaths;

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
    EnableSuccessOnReturnCode(_expectedReturnCode);

    if (!IsInitialized())
    {
        // 2. Check if command is found in search path collection that was provided
        vector<string>::const_iterator it=appSearchPaths.begin();
        vector<string>::const_iterator end=appSearchPaths.end();
        for (; it!=end; it++)
        {
            string currentName = *it + "/" + commandName;
            bool commandExists = FileTools::FileExists(currentName);
            if (commandExists)
            {
                commandName = currentName;
                break;
            }
        }
    }
}

bool ConsoleJob::IsInitialized()
{
    // 1. Check if command is a valid file, considering current and full path.
    bool commandExists = FileTools::FileExists(commandName);
    if (!commandExists)
    {
        if (!commandExists)
        {
            string output("");
            // 2. Check if command is a global command
            string checkingCommand("which");
            checkingCommand += string(" ") + commandName;
            int returnCode = Tools::RunExternalCommand(checkingCommand, output);
            commandExists = (returnCode == 0);
        }
    }
    return (commandName != "" && commandExists);
}

// @TODO resolve bug where apparently output is not correctly processed with it has some special chars (like in :-) )
JobStatus *ConsoleJob::Run()
{
    JobStatus* status = new JobStatus(JobStatus::OK);
    const string fullCommand = CreateFullCommand();

    if (outputFileName != "")
    {
        receivedReturnCode = Tools::RunExternalCommandFile(fullCommand, outputFileName);
        status->AddFile(outputFileName);
    }
    else
        receivedReturnCode = Tools::RunExternalCommand(fullCommand, standardOutput, true);

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
        int returnValue = Tools::RunExternalCommand(parserCommand, miniDescription);
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


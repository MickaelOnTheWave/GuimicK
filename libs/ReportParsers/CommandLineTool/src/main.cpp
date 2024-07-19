#include <iostream>

#include <commandlinemanager.h>
#include <abstractoutputparser.h>
#include <aptgetupgradeparser.h>
#include <clamavreportparser.h>
#include <dfcommandparser.h>
#include <gitporcelainreportparser.h>
#include <rsnapshotreportparser.h>

using namespace std;

static const int OK             = 0;
static const int PARSER_ERROR   = 1;
static const int PARSING_ERROR  = 2;
static const int USAGE_ERROR    = 3;

AbstractOutputParser* CreateParser(const std::wstring& name);

int main(int argc, char* argv[])
{
    CommandLineManager commandLine(argc, argv);

    commandLine.AddParameter(L"input", L"[REPORT_FILE]");
    commandLine.AddParameter(L"output", L"[OUTPUT_FILE]");
    commandLine.AddParameter(L"parser", L"[PARSER]");
    commandLine.AddParameter(L"direct", L"Outputs raw data without \"headers\". For automated usage.");
    commandLine.AddParameter(L"inputbuffer", L"Uses input directly from command line instead of file. "
                             L"Must be between \"\".");

    bool isDirectUsage = commandLine.HasParameter(L"direct");

    if (!isDirectUsage)
    {
        commandLine.EnableHelpCommand();
        if (commandLine.HandleHelpCommand())
            return OK;
        else if (commandLine.HandleUnknownParameters())
            return USAGE_ERROR;
    }

    const wstring inputFile(commandLine.GetParameterValue(L"input"));
    const wstring inputBuffer(commandLine.GetParameterValue(L"inputbuffer"));
    const wstring outputFile(commandLine.GetParameterValue(L"output"));
    const wstring parserAlgorithm(commandLine.GetParameterValue(L"parser"));
    const bool isInputValid = (inputFile != L"" || inputBuffer != L"");

    if (isInputValid == false || parserAlgorithm == L"")
    {
        if (!isDirectUsage)
            commandLine.ShowUsageInformation();
        return USAGE_ERROR;
    }

    AbstractOutputParser* parser = CreateParser(parserAlgorithm);
    if (parser == NULL)
    {
        if (!isDirectUsage)
            wcout << L"Error : Parser " << parserAlgorithm << L"is not handled." << endl;
        return PARSER_ERROR;
    }


    bool result = false;
    if (inputFile != L"")
        result = parser->ParseFile(inputFile);
    else
        result = parser->ParseBuffer(inputBuffer);

    if (result == false)
    {
        if (!isDirectUsage)
            cout << "Error while parsing." << endl;
        return PARSING_ERROR;
    }

    parser->WriteFullDescriptionToFile(outputFile);

    if (!isDirectUsage)
        cout << "Mini description : ";
     wcout << parser->GetMiniDescription() << endl;

    return OK;
}

AbstractOutputParser* CreateParser(const std::wstring& name)
{
    if (name == L"aptupgrade")
        return new AptGetUpgradeParser();
    else if (name == L"clamav")
        return new ClamAvReportParser();
    else if (name == L"rsnapshot")
        return new RSnapshotReportParser();
    else if (name == L"git")
        return new GitPorcelainReportParser();
    else if (name == L"df")
        return new DfCommandParser();
    else
        return NULL;
}

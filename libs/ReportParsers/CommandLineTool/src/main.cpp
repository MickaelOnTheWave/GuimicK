#include <iostream>

#include <commandlinemanager.h>
#include <abstractoutputparser.h>
#include <aptgetupgradeparser.h>
#include <clamavreportparser.h>
#include <dfcommandparser.h>
#include <gitporcelainreportparser.h>
#include <rsnapshotreportparser.h>

#include "stringtools.h"

using namespace std;

static const int OK             = 0;
static const int PARSER_ERROR   = 1;
static const int PARSING_ERROR  = 2;
static const int USAGE_ERROR    = 3;

AbstractOutputParser* CreateParser(const std::string& name);

int main(int argc, char* argv[])
{
    CommandLineManager commandLine(argc, argv);

    commandLine.AddParameter("input", "[REPORT_FILE]");
    commandLine.AddParameter("output", "[OUTPUT_FILE]");
    commandLine.AddParameter("parser", "[PARSER]");
    commandLine.AddParameter("direct", "Outputs raw data without \"headers\". For automated usage.");
    commandLine.AddParameter("inputbuffer", "Uses input directly from command line instead of file. "
                             "Must be between \"\".");

    bool isDirectUsage = commandLine.HasParameter("direct");

    if (!isDirectUsage)
    {
        commandLine.EnableHelpCommand();
        if (commandLine.HandleHelpCommand())
            return OK;
        else if (commandLine.HandleUnknownParameters())
            return USAGE_ERROR;
    }

    const string inputFile(commandLine.GetParameterValue("input"));
    const string inputBuffer(commandLine.GetParameterValue("inputbuffer"));
    const string outputFile(commandLine.GetParameterValue("output"));
    const string parserAlgorithm(commandLine.GetParameterValue("parser"));
    const bool isInputValid = (inputFile != "" || inputBuffer != "");

    if (isInputValid == false || parserAlgorithm == "")
    {
        if (!isDirectUsage)
            commandLine.ShowUsageInformation();
        return USAGE_ERROR;
    }

    AbstractOutputParser* parser = CreateParser(parserAlgorithm);
    if (parser == NULL)
    {
        if (!isDirectUsage)
            cout << "Error : Parser " << parserAlgorithm << "is not handled." << endl;
        return PARSER_ERROR;
    }


    bool result = false;
    if (inputFile != "")
       result = parser->ParseFile(StringTools::Utf8ToUnicode(inputFile));
    else
       result = parser->ParseBuffer(StringTools::Utf8ToUnicode(inputBuffer));

    if (result == false)
    {
        if (!isDirectUsage)
            cout << "Error while parsing." << endl;
        return PARSING_ERROR;
    }

    parser->WriteFullDescriptionToFile(StringTools::Utf8ToUnicode(outputFile));

    if (!isDirectUsage)
        cout << "Mini description : ";
     wcout << parser->GetMiniDescription() << endl;

    return OK;
}

AbstractOutputParser* CreateParser(const std::string& name)
{
    if (name == "aptupgrade")
        return new AptGetUpgradeParser();
    else if (name == "clamav")
        return new ClamAvReportParser();
    else if (name == "rsnapshot")
        return new RSnapshotReportParser();
    else if (name == "git")
        return new GitPorcelainReportParser();
    else if (name == "df")
        return new DfCommandParser();
    else
        return NULL;
}

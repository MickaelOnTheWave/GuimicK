#include <iostream>

#include <commandlinemanager.h>
#include <abstractoutputparser.h>
#include <aptgetupgradeparser.h>
#include <clamavreportparser.h>
#include <gitreportparser.h>
#include <rsnapshotreportparser.h>

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
    const string outputFile(commandLine.GetParameterValue("output"));
    const string parserAlgorithm(commandLine.GetParameterValue("parser"));

    if (inputFile== "" || parserAlgorithm == "")
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

    bool result = parser->ParseFile(inputFile);
    if (result == false)
    {
        if (!isDirectUsage)
            cout << "Error while parsing." << endl;
        return PARSING_ERROR;
    }

    parser->WriteFullDescriptionToFile(outputFile);

    if (!isDirectUsage)
        cout << "Mini description : ";
     cout << parser->GetMiniDescription() << endl;

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
        return new GitReportParser();
    else
        return NULL;
}

#include "gitplumbingreportparser.h"

#include "tools.h"

using namespace std;

GitPlumbingReportParser::GitPlumbingReportParser()
    : AbstractFileBackupParser(new FileBackupReport())
{
}

GitPlumbingReportParser::~GitPlumbingReportParser()
{
}

bool GitPlumbingReportParser::ParseBuffer(const string &buffer)
{
    reportData->Clear();

    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    FillReportData(lines);
    return true;
}

void GitPlumbingReportParser::GetReport(FileBackupReport &report)
{
    report = *reportData;
}

void GitPlumbingReportParser::FillReportData(const std::vector<string> &lines)
{
    vector<string>::const_iterator it=lines.begin();
    for (; it!=lines.end(); ++it)
    {
        vector<string> tokens;
        Tools::TokenizeString(*it, '\t', tokens);
        if (tokens.front() == "A")
            reportData->AddAsAdded(tokens.back());
        else if (tokens.front() == "D")
            reportData->AddAsRemoved(tokens.back());
        else
            reportData->AddAsModified(tokens.back());
    }
}

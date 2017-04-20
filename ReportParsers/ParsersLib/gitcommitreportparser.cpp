#include "gitcommitreportparser.h"

#include "tools.h"

using namespace std;

GitCommitReportParser::GitCommitReportParser()
    : AbstractFileBackupParser(new FileBackupReport())
{
}

GitCommitReportParser::~GitCommitReportParser()
{
}

bool GitCommitReportParser::ParseBuffer(const string &buffer)
{
    reportData->Clear();

    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    FillReportData(lines);
    return true;
}

void GitCommitReportParser::GetReport(FileBackupReport &report)
{
    report = *reportData;
}

void GitCommitReportParser::FillReportData(const std::vector<string> &lines)
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

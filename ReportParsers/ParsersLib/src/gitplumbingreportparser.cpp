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

bool GitPlumbingReportParser::ParseBuffer(const wstring &buffer)
{
    reportData->Clear();

    vector<wstring> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    FillReportData(lines);
    return true;
}

void GitPlumbingReportParser::GetReport(FileBackupReport &report)
{
    report = *reportData;
}

void GitPlumbingReportParser::FillReportData(const vector<wstring> &lines)
{
    vector<wstring>::const_iterator it=lines.begin();
    for (; it!=lines.end(); ++it)
    {
        vector<wstring> tokens;
        Tools::TokenizeString(*it, '\t', tokens);
        if (tokens.front() == L"A")
            reportData->AddAsAdded(tokens.back());
        else if (tokens.front() == L"D")
            reportData->AddAsRemoved(tokens.back());
        else
            reportData->AddAsModified(tokens.back());
    }
}

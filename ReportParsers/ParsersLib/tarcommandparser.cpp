#include "tarcommandparser.h"

#include "tools.h"

using namespace std;

TarCommandParser::TarCommandParser() : AbstractFileBackupParser(new FileBackupReport())
{
}

TarCommandParser::~TarCommandParser()
{
}

bool TarCommandParser::ParseBuffer(const std::string &buffer)
{
    reportData->Clear();

    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    FillReportData(lines);

    return true;
}

void TarCommandParser::GetReport(FileBackupReport &_reportData)
{
    _reportData = *reportData;
}

void TarCommandParser::FillReportData(const std::vector<string> &lines)
{
    const string sourceFolder = lines.front();
    vector<string>::const_iterator it=lines.begin()+1;
    for (; it!=lines.end(); ++it)
    {
        if (sourceFolder.size() < it->size())
        {
            const string currentFile = it->substr(sourceFolder.size());
            reportData->AddAsAdded(currentFile);
        }
    }
}

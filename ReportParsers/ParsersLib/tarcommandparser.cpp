#include "tarcommandparser.h"

#include "tools.h"

using namespace std;

TarCommandParser::TarCommandParser(const string &_tarCommand)
    : AbstractFileBackupParser(new FileBackupReport()),
      tarCommand(_tarCommand)
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
    int filesStartPosition = 1;
    string sourceFolder = lines.front();
    if (sourceFolder.find(tarCommand) == 0)
    {
        sourceFolder = *(lines.begin()+1);
        ++filesStartPosition;
    }

    vector<string>::const_iterator it=lines.begin()+filesStartPosition;
    for (; it!=lines.end(); ++it)
    {
        const string currentFile = it->substr(sourceFolder.size());
        reportData->AddAsAdded(currentFile);
    }
}

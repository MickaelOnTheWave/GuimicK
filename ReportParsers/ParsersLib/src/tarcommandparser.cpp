#include "tarcommandparser.h"

#include "stringtools.h"

using namespace std;

TarCommandParser::TarCommandParser(const wstring &_tarCommand)
    : AbstractFileBackupParser(new FileBackupReport()),
      tarCommand(_tarCommand)
{
}

TarCommandParser::~TarCommandParser()
{
}

bool TarCommandParser::ParseBuffer(const std::wstring &buffer)
{
    reportData->Clear();

    vector<wstring> lines;
    StringTools::Tokenize(buffer, '\n', lines);

    FillReportData(lines);

    return true;
}

void TarCommandParser::GetReport(FileBackupReport &_reportData)
{
    _reportData = *reportData;
}

void TarCommandParser::FillReportData(const std::vector<wstring> &lines)
{
    int filesStartPosition = 1;
    wstring sourceFolder = lines.front();
    if (tarCommand != L"" && sourceFolder.find(tarCommand) == 0)
    {
        sourceFolder = *(lines.begin()+1);
        ++filesStartPosition;
    }

    vector<wstring>::const_iterator it=lines.begin()+filesStartPosition;
    for (; it!=lines.end(); ++it)
    {
        if (it->size() > sourceFolder.size())
        {
            const wstring currentFile = it->substr(sourceFolder.size());
            reportData->AddAsAdded(currentFile);
        }
    }
}

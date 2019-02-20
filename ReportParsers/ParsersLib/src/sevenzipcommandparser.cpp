#include "sevenzipcommandparser.h"

#include "tools.h"

using namespace std;

SevenZipCommandParser::SevenZipCommandParser()
    : AbstractFileBackupParser(new FileBackupReport())
{
}


SevenZipCommandParser::~SevenZipCommandParser()
{
}

bool SevenZipCommandParser::ParseBuffer(const std::string &buffer)
{
    reportData->Clear();

    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    FillReportData(lines);

    return true;
}

void SevenZipCommandParser::GetReport(FileBackupReport &_reportData)
{
    _reportData = *reportData;
}

void SevenZipCommandParser::FillReportData(const std::vector<string> &lines)
{
    int filesStartPosition = 1;
    string sourceFolder = lines.front();
/*    if (tarCommand != "" && sourceFolder.find(tarCommand) == 0)
    {
        sourceFolder = *(lines.begin()+1);
        ++filesStartPosition;
    }

    vector<string>::const_iterator it=lines.begin()+filesStartPosition;
    for (; it!=lines.end(); ++it)
    {
        if (it->size() > sourceFolder.size())
        {
            const string currentFile = it->substr(sourceFolder.size());
            reportData->AddAsAdded(currentFile);
        }
    }*/
}

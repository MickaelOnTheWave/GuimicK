#include "sevenzipcommandparser.h"

#include "tools.h"

using namespace std;

const char addChar = '+';
const char removeChar = '-';
const char updateChar = 'U';

bool IsFileLine(const wstring& lineData)
{
   const char firstChar = lineData.front();
   const bool isFirstCharValid = (firstChar == addChar ||
                                  firstChar == removeChar ||
                                  firstChar == updateChar);
   const bool isSecondCharEmpty = (lineData.at(1) == ' ');
   return isFirstCharValid && isSecondCharEmpty;
}

wstring ExtractFilename(const wstring& lineData)
{
   return lineData.substr(2);
}

SevenZipCommandParser::SevenZipCommandParser()
    : AbstractFileBackupParser(new FileBackupReport())
{
}


SevenZipCommandParser::~SevenZipCommandParser()
{
}

bool SevenZipCommandParser::ParseBuffer(const std::wstring &buffer)
{
    reportData->Clear();

    vector<wstring> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    FillReportData(lines);

    return true;
}

void SevenZipCommandParser::GetReport(FileBackupReport &_reportData)
{
    _reportData = *reportData;
}

void SevenZipCommandParser::FillReportData(const vector<wstring> &lines)
{
   vector<wstring>::const_iterator it = lines.begin();
   for (; it != lines.end(); ++it)
   {
      if (!it->empty() && IsFileLine(*it))
      {
         const char firstChar = it->front();
         if (firstChar == updateChar)
            reportData->AddAsModified(ExtractFilename(*it));
         else if (firstChar == addChar)
            reportData->AddAsAdded(ExtractFilename(*it));
         else if (firstChar == removeChar)
            reportData->AddAsRemoved(ExtractFilename(*it));
      }
   }
}

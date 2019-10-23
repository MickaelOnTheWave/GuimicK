#include "sevenzipcommandparser.h"

#include "stringtools.h"

using namespace std;

const wchar_t addChar = '+';
const wchar_t removeChar = '-';
const wchar_t updateChar = 'U';

bool IsFileLine(const wstring& lineData)
{
   const wchar_t firstChar = *lineData.begin();
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
    StringTools::Tokenize(buffer, '\n', lines);

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
         UpdateReportData(it);
   }
}

void SevenZipCommandParser::UpdateReportData(vector<wstring>::const_iterator& it)
{
   const wchar_t firstChar = *it->begin();
   if (firstChar == updateChar)
      reportData->AddAsModified(ExtractFilename(*it));
   else if (firstChar == addChar)
      reportData->AddAsAdded(ExtractFilename(*it));
   else if (firstChar == removeChar)
      reportData->AddAsRemoved(ExtractFilename(*it));
}

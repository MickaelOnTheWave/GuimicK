#include "chkdskcommandparser.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "bytestring.h"
#include "stringtools.h"

using namespace std;

static const wstring BadSectorsMessage = L"Bad sectors detected";
static const wstring DriveOkMessage = L"Drive is OK";
static const wstring NoDriveMessage = L"No drive tested";

typedef vector<wstring> StringLines;

void CreateReportSections(const StringLines& reportLines,
                          vector<StringLines>& reportSections)
{
   const wstring sectionSeparator = L"";
   StringLines currentSection;
   StringLines::const_iterator it = reportLines.begin();
   for(; it!=reportLines.end(); ++it)
   {
      if (*it == sectionSeparator)
      {
         if (currentSection.empty() == false)
            reportSections.push_back(currentSection);
         currentSection.clear();
      }
      else
         currentSection.push_back(*it);
   }
}

bool HasMarker(const wstring& line, const wstring& marker)
{
   return (line.find(marker) != wstring::npos);
}

wstring GetFirstNonEmptyToken(const StringLines& tokens)
{
   StringLines::const_iterator it = tokens.begin();
   for (; it != tokens.end(); ++it)
   {
      if (*it != L"")
         return *it;
   }
   return L"";
}

long long GetSizeValueFromLine(const wstring& line)
{
   vector<wstring> tokens;
   StringTools::Tokenize(line, L' ', tokens);

   const wstring sizeString = GetFirstNonEmptyToken(tokens);
   if (sizeString != L"")
      return StringTools::ToLongLong(sizeString) * 1024;
   else
      return -1;
}

wstring GetSizeStringFromLine(const wstring& line)
{
   const long long value = GetSizeValueFromLine(line);
   if (value != -1)
      return ByteString<wstring>::Format(value);
   else
      return L"";
}

void SetTotalSpace(LogicalDrive& drive, const wstring& line)
{
   drive.totalSpace = GetSizeStringFromLine(line);
}

void SetFreeSpace(LogicalDrive& drive, const wstring& line)
{
   drive.freeSpace = GetSizeStringFromLine(line);
}

void SetBadSectors(LogicalDrive& drive, const wstring& line)
{
   const long long byteValue = GetSizeValueFromLine(line);
   drive.hasBadSectors = (byteValue > 0);
   drive.badSectors = ByteString<wstring>::Format(byteValue);
}

wstring DriveWord(const size_t count)
{
   return (count == 1) ? L" drive " : L" drives ";
}

bool IsDriveFailing(const LogicalDrive& drive)
{
   return drive.HasBadSectors();
}


ChkdskCommandParser::ChkdskCommandParser()
{
}

bool ChkdskCommandParser::ParseBuffer(const wstring &buffer)
{
   StringLines tokens;
   StringTools::Tokenize(buffer, L'\n', tokens);

   vector<StringLines> sections;
   CreateReportSections(tokens, sections);

   bool ok = (sections.empty() == false);
   if (ok)
      ok = ParseSummarySection(sections.back());
   return ok;
}

wstring ChkdskCommandParser::GetMiniDescription()
{
   if (driveList.empty())
      return NoDriveMessage;
   else if (driveList.size() == 1)
      return CreateOneDriveDescription();
   else
      return CreateMultiDriveDescription();
}

wstring ChkdskCommandParser::GetFullDescription()
{
   const wstring tab = L"\t";
   const wstring eol = L"\n";
   wstring fullDescription;
   vector<LogicalDrive>::const_iterator it = driveList.begin();
   for (; it != driveList.end(); ++it)
   {
      fullDescription += L"Drive " + it->name + L":" + eol;
      fullDescription += tab + L"Total Space : " + it->totalSpace + eol;
      fullDescription += tab + L"Free Space : " + it->freeSpace + eol;
      fullDescription += tab + L"Bad Sectors : " + it->badSectors + eol;
      fullDescription += eol;
   }
   return fullDescription;
}

void ChkdskCommandParser::GetReportLines(const vector<wstring> &input,
                                         vector<wstring> &output) const
{
    vector<wstring>::const_iterator it=input.begin();
    bool foundReportInit = false;
    for (; it != input.end(); ++it)
    {
        if (it->find(L"Windows has checked the filesystem") == 0)
            foundReportInit = true;

        if (foundReportInit)
            output.push_back(*it);
    }
}

bool ChkdskCommandParser::ParseSummarySection(const vector<wstring>& summary)
{
   const wstring totalSpaceMarker = L"total disk space";
   const wstring freeSpaceMarker = L"available on disk";
   const wstring badSectorsMarker = L"bad sectors";

   LogicalDrive drive;

   StringLines::const_iterator it = summary.begin();
   for(; it!=summary.end(); ++it)
   {
      if (HasMarker(*it, totalSpaceMarker))
         SetTotalSpace(drive, *it);
      else if (HasMarker(*it, freeSpaceMarker))
         SetFreeSpace(drive, *it);
      else if (HasMarker(*it, badSectorsMarker))
         SetBadSectors(drive, *it);
   }

   const bool isDriveOk = (
         drive.badSectors != L"" &&
         drive.freeSpace != L"" &&
         drive.totalSpace != L""
   );
   if (isDriveOk)
      driveList.push_back(drive);

   return isDriveOk;
}

bool ChkdskCommandParser::CreateDriveDataFromReport(const vector<wstring>& reportLines)
{
   return true;
}

wstring ChkdskCommandParser::CreateOneDriveDescription() const
{
   const LogicalDrive drive = GetFirstDrive();
   if (drive.HasBadSectors())
      return BadSectorsMessage;
   else
      return DriveOkMessage;
}

wstring ChkdskCommandParser::CreateMultiDriveDescription() const
{
   if (AllDrivesOk())
      return CreateAllDrivesOkMessage();
   else if (AllDrivesFailing())
      return CreateAllDrivesFailingMessage();
   else
      return CreateMixedResultsMessage();
}

bool ChkdskCommandParser::AllDrivesOk() const
{
   none_of(driveList.begin(), driveList.end(), IsDriveFailing);
}

bool ChkdskCommandParser::AllDrivesFailing() const
{
   all_of(driveList.begin(), driveList.end(), IsDriveFailing);
}

wstring ChkdskCommandParser::CreateAllDrivesOkMessage() const
{
   wstringstream stream;
   stream << driveList.size() << L" drives OK";
   return stream.str();
}

wstring ChkdskCommandParser::CreateAllDrivesFailingMessage() const
{
   wstringstream stream;
   stream << driveList.size() << L" drives failing";
   return stream.str();
}

wstring ChkdskCommandParser::CreateMixedResultsMessage() const
{
   const int failCount = count_if(driveList.begin(), driveList.end(), IsDriveFailing);
   const int okCount = driveList.size() - failCount;

   wstringstream stream;
   stream << okCount << DriveWord(okCount) << L"ok, ";
   stream << failCount << DriveWord(failCount) << L"failing.";
   return stream.str();
}



#include "chkdskcommandparser.h"

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

wstring GetSizeStringFromLine(const wstring& line)
{
   vector<wstring> tokens;
   StringTools::Tokenize(line, L' ', tokens);

   const wstring sizeString = GetFirstNonEmptyToken(tokens);
   if (sizeString != L"")
   {
      long long sizeValue = StringTools::ToLongLong(sizeString) * 1024;
      return ByteString<wstring>::Format(sizeValue);
   }
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
   drive.badSectors = GetSizeStringFromLine(line);
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

std::wstring ChkdskCommandParser::GetMiniDescription()
{
   if (driveList.empty())
      return NoDriveMessage;
   else if (GetFirstDrive().HasBadSectors())
      return BadSectorsMessage;
   else
      return DriveOkMessage;
}

std::wstring ChkdskCommandParser::GetFullDescription()
{
    return L"";
}

void ChkdskCommandParser::GetReportLines(const std::vector<wstring> &input,
                                         std::vector<wstring> &output) const
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


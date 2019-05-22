#include "chkdskcommandparser.h"

#include <string>
#include <vector>

#include "stringtools.h"

using namespace std;

static const wstring BadSectorsMessage = L"Bad sectors detected";
static const wstring DriveOkMessage = L"Drive is OK";

ChkdskCommandParser::ChkdskCommandParser()
{
}

bool ChkdskCommandParser::ParseBuffer(const wstring &buffer)
{
    vector<wstring> tokens;
    StringTools::Tokenize(buffer, L'\n', tokens);

    vector<wstring> reportLines;
    GetReportLines(tokens, reportLines);

    return CreateDriveDataFromReport(reportLines);
}

std::wstring ChkdskCommandParser::GetMiniDescription()
{
    if (drive.HasBadSectors())
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

bool ChkdskCommandParser::CreateDriveDataFromReport(const vector<wstring>& reportLines)
{
    return true;
}


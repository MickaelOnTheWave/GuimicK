#include "chkdskcommanderrorparser.h"

#include <string>

using namespace std;

const wstring NoAdminRightsMessage = L"Requires admin priviledges";
const wstring InvalidDriveMessage = L"Drive not recognized";
const wstring UnknownErrorMessage = L"check report file";

ChkdskCommandErrorParser::ChkdskCommandErrorParser()
    : miniDescription(L""), fullDescription(L"")
{

}

bool ChkdskCommandErrorParser::ParseBuffer(const wstring &buffer)
{
    if (IsNoAdminRightsError(buffer))
        miniDescription = NoAdminRightsMessage;
    else if (IsInvalidDriveError(buffer))
        miniDescription = InvalidDriveMessage;
    else
    {
        miniDescription = UnknownErrorMessage;
        fullDescription = buffer;
    }
    return true;
}

std::wstring ChkdskCommandErrorParser::GetMiniDescription()
{
    return miniDescription;
}

std::wstring ChkdskCommandErrorParser::GetFullDescription()
{
    return fullDescription;
}

bool ChkdskCommandErrorParser::IsNoAdminRightsError(const wstring &buffer) const
{
    const wstring NoAdminRights = L"Access Denied";
    const bool startsWithNoAdmin = (buffer.find(NoAdminRights) == 0);
    const bool hasPriviledgeWord = (buffer.find(L"privileges") != string::npos);
    return (startsWithNoAdmin && hasPriviledgeWord);
}

bool ChkdskCommandErrorParser::IsInvalidDriveError(const wstring &buffer) const
{
    return (buffer.find(L"Cannot open volume") == 0);
}

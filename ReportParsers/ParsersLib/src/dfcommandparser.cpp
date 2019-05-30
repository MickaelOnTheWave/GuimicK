#include "dfcommandparser.h"

#include <sstream>
#include "stringtools.h"

using namespace std;

wstring CreateFormattedSize(const wstring &rawSize)
{
    const size_t lastCharPos = rawSize.size()-1;
    wstring formattedSize = rawSize.substr(0, lastCharPos);
    formattedSize += wstring(L" ") + rawSize[lastCharPos] + L"b";
    return formattedSize;
}

LogicalDrive BuildDriveFromTokens(const vector<wstring> &properties)
{
   LogicalDrive drive;
   drive.name = properties[0];
   drive.totalSpace = CreateFormattedSize(properties[1]);
   drive.usedSpace = CreateFormattedSize(properties[2]);
   drive.freeSpace = CreateFormattedSize(properties[3]);
   drive.ratio = properties[4];
   return drive;
}


DfCommandParser::DfCommandParser()
{
}

bool DfCommandParser::ParseBuffer(const wstring& buffer)
{
    driveList.clear();

    vector<wstring> lines;
    StringTools::Tokenize(buffer, '\n', lines);

    if (lines.size() > 1)
        return FillDriveData(lines);
    else
        return false;
}

wstring DfCommandParser::GetMiniDescription()
{
    if (driveList.size() == 1)
        return CreateResumedMiniDescription(driveList.front());
    else
        return CreateDriveListDescription();
}

wstring DfCommandParser::GetFullDescription()
{
    if (driveList.size() < 2)
        return L"";
    else
        return CreateFullDescription();
}

bool DfCommandParser::FillDriveData(const std::vector<wstring> &lines)
{
    std::vector<wstring>::const_iterator it=lines.begin()+1;
    for (; it!=lines.end(); ++it)
    {
        vector<wstring> tokens;
        TokenizeUsingWhitespaces(*it, tokens);

        if (IsDesirableDriveName(tokens[0]))
            driveList.push_back(BuildDriveFromTokens(tokens));
    }
    return (!driveList.empty());
}

void DfCommandParser::TokenizeUsingWhitespaces(const wstring &buffer,
                                               vector<wstring> &tokens) const
{
    tokens.clear();
    wstring currentToken(L"");
    for (unsigned int i=0; i<buffer.length(); ++i)
    {
        if (isspace(buffer[i]) != 0)
        {
            if (currentToken != L"")
            {
                tokens.push_back(currentToken);
                currentToken = L"";
            }
        }
        else
            currentToken += buffer[i];
    }
}

bool DfCommandParser::IsDesirableDriveName(const wstring &name) const
{
    return (name.length() > 0 && name[0] == '/');
}

wstring DfCommandParser::CreateResumedMiniDescription(const LogicalDrive& drive) const
{
    return drive.freeSpace + L" available (" + drive.ratio + L" used)";
}

wstring DfCommandParser::CreateDriveListDescription() const
{
    wstringstream stream;
    stream << driveList.size() << " drives checked, see report";
    return stream.str();
}

wstring DfCommandParser::CreateFullDescription() const
{
    wstringstream stream;
    vector<LogicalDrive>::const_iterator it = driveList.begin();
    for (;it != driveList.end(); ++it)
    {
        stream << it->name << " : " << it->totalSpace << " total, ";
        stream << CreateResumedMiniDescription(*it) << endl;
    }

    return stream.str();
}


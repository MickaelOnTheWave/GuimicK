#include "dfcommandparser.h"

#include <sstream>
#include "stringtools.h"

using namespace std;

Drive::Drive()
    : name(L""), totalSpace(L""), usedSpace(L""), freeSpace(L""), ratio(L"")
{
}

Drive::Drive(const vector<wstring> &properties)
{
    name = properties[0];
    totalSpace = CreateFormattedSize(properties[1]);
    usedSpace = CreateFormattedSize(properties[2]);
    freeSpace = CreateFormattedSize(properties[3]);
    ratio = properties[4];
}



wstring Drive::CreateFormattedSize(const wstring &rawSize) const
{
    const size_t lastCharPos = rawSize.size()-1;
    wstring formattedSize = rawSize.substr(0, lastCharPos);
    formattedSize += wstring(L" ") + rawSize[lastCharPos] + L"b";
    return formattedSize;
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
    {
        FillDriveData(lines);
        return true;
    }
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

void DfCommandParser::GetDrives(std::vector<Drive> &drives) const
{
    copy(driveList.begin(), driveList.end(), back_inserter(drives));
}

Drive DfCommandParser::GetFirstDrive() const
{
    return driveList.front();
}

void DfCommandParser::FillDriveData(const std::vector<wstring> &lines)
{
    std::vector<wstring>::const_iterator it=lines.begin()+1;
    for (; it!=lines.end(); ++it)
    {
        vector<wstring> tokens;
        TokenizeUsingWhitespaces(*it, tokens);

        if (IsDesirableDriveName(tokens[0]))
            driveList.push_back(Drive(tokens));
    }
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

wstring DfCommandParser::CreateResumedMiniDescription(const Drive &drive) const
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
    vector<Drive>::const_iterator it = driveList.begin();
    for (;it != driveList.end(); ++it)
    {
        stream << it->name << " : " << it->totalSpace << " total, ";
        stream << CreateResumedMiniDescription(*it) << endl;
    }

    return stream.str();
}


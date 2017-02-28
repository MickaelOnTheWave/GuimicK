#include "dfcommandparser.h"

#include "tools.h"

using namespace std;

Drive::Drive()
    : name(""), totalSpace(""), usedSpace(""), freeSpace(""), ratio("")
{
}

Drive::Drive(const std::vector<string> &properties)
{
    name = properties[0];
    totalSpace = CreateFormattedSize(properties[1]);
    usedSpace = CreateFormattedSize(properties[2]);
    freeSpace = CreateFormattedSize(properties[3]);
    ratio = properties[4];
}

string Drive::CreateFormattedSize(const string &rawSize) const
{
    const size_t lastCharPos = rawSize.size()-1;
    string formattedSize = rawSize.substr(0, lastCharPos);
    formattedSize += string(" ") + rawSize[lastCharPos] + "b";
    return formattedSize;
}


DfCommandParser::DfCommandParser()
{
}

bool DfCommandParser::ParseBuffer(const string& buffer)
{
    driveList.clear();

    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    if (lines.size() > 1)
    {
        FillDriveData(lines);
        return true;
    }
    else
        return false;
}

string DfCommandParser::GetMiniDescription()
{
    return "mini description";
}

string DfCommandParser::GetFullDescription()
{
    return "full description";
}

void DfCommandParser::GetDrives(std::vector<Drive> &drives) const
{
    copy(driveList.begin(), driveList.end(), back_inserter(drives));
}

Drive DfCommandParser::GetFirstDrive() const
{
    return driveList.front();
}

void DfCommandParser::FillDriveData(const std::vector<string> &lines)
{
    std::vector<string>::const_iterator it=lines.begin()+1;
    for (; it!=lines.end(); ++it)
    {
        vector<string> tokens;
        TokenizeUsingWhitespaces(*it, tokens);

        if (IsDesirableDriveName(tokens[0]))
            driveList.push_back(Drive(tokens));
    }
}

void DfCommandParser::TokenizeUsingWhitespaces(const string &buffer,
                                               vector<string> &tokens) const
{
    tokens.clear();
    string currentToken("");
    for (unsigned int i=0; i<buffer.length(); ++i)
    {
        if (isspace(buffer[i]) != 0)
        {
            if (currentToken != "")
            {
                tokens.push_back(currentToken);
                currentToken = "";
            }
        }
        else
            currentToken += buffer[i];
    }
}

bool DfCommandParser::IsDesirableDriveName(const string &name) const
{
    return (name.length() > 0 && name[0] == '/');
}


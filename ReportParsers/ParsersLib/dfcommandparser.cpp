#include "dfcommandparser.h"

using namespace std;

DfCommandParser::DfCommandParser()
{
}

bool DfCommandParser::ParseBuffer(const string& buffer)
{
    return (buffer != "Wrong buffer");
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

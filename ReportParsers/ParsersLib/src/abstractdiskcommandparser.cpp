#include "abstractdiskcommandparser.h"

using namespace std;

void AbstractDiskCommandParser::GetDrives(vector<LogicalDrive>& drives) const
{
    copy(driveList.begin(), driveList.end(), back_inserter(drives));
}

LogicalDrive AbstractDiskCommandParser::GetFirstDrive() const
{
    return driveList.front();
}

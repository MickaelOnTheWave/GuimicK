#ifndef ABSTRACTDISKCOMMANDPARSER_H
#define ABSTRACTDISKCOMMANDPARSER_H

#include "abstractoutputparser.h"
#include "logicaldrive.h"
#include <vector>

class AbstractDiskCommandParser : public AbstractOutputParser
{
public:
    void GetDrives(std::vector<LogicalDrive>& drives) const;
    LogicalDrive GetFirstDrive() const;

protected:
    std::vector<LogicalDrive> driveList;
};

#endif // ABSTRACTDISKCOMMANDPARSER_H

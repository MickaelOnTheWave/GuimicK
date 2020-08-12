#ifndef ABSTRACTDISKCOMMANDPARSER_H
#define ABSTRACTDISKCOMMANDPARSER_H

#include "abstractoutputparser.h"
#include "logicaldrive.h"
#include <vector>

struct ErrorDrive
{
   static const int UNDEFINED = 0;
   static const int NOT_FOUND = 1;

   std::wstring name;
   int errorType;
};

class AbstractDiskCommandParser : public AbstractOutputParser
{
public:
    void GetDrives(std::vector<LogicalDrive>& drives) const;
    LogicalDrive GetFirstDrive() const;

protected:
    std::vector<LogicalDrive> driveList;
    std::vector<ErrorDrive> errorDriveList;
};

#endif // ABSTRACTDISKCOMMANDPARSER_H

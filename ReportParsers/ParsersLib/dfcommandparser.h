#ifndef DFCOMMANDPARSER_H
#define DFCOMMANDPARSER_H

#include "abstractoutputparser.h"

#include <vector>

class Drive
{
public :
    std::string name;
    std::string totalSpace;
    std::string usedSpace;
    std::string freeSpace;
    std::string ratio;
};

class DfCommandParser : public AbstractOutputParser
{
public:
    DfCommandParser();

    virtual bool ParseBuffer(const std::string& buffer);
    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();

    void GetDrives(std::vector<Drive>& drives) const;
    Drive GetFirstDrive() const;

private:
    std::vector<Drive> driveList;
};

#endif // DFCOMMANDPARSER_H

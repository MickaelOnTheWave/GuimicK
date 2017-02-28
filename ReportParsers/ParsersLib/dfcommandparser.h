#ifndef DFCOMMANDPARSER_H
#define DFCOMMANDPARSER_H

#include "abstractoutputparser.h"

#include <vector>

class Drive
{
public :
    Drive();
    Drive(const std::vector<std::string>& properties);

    std::string name;
    std::string totalSpace;
    std::string usedSpace;
    std::string freeSpace;
    std::string ratio;

private :
    std::string CreateFormattedSize(const std::string& rawSize) const;
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
    void FillDriveData(const std::vector<std::string>& lines);
    void TokenizeUsingWhitespaces(const std::string& buffer,
                                  std::vector<std::string>& tokens) const;
    bool IsDesirableDriveName(const std::string& name) const;

    std::string CreateResumedMiniDescription(const Drive& drive) const;
    std::string CreateDriveListDescription() const;
    std::string CreateFullDescription() const;

    std::vector<Drive> driveList;
};

#endif // DFCOMMANDPARSER_H

#ifndef DFCOMMANDPARSER_H
#define DFCOMMANDPARSER_H

#include "abstractoutputparser.h"

#include <vector>

class Drive
{
public :
    Drive();
    Drive(const std::vector<std::wstring>& properties);
    Drive(const std::wstring& _name,
          const std::wstring& _totalSpace,
          const std::wstring& _usedSpace,
          const std::wstring& _ratio);

    std::wstring name;
    std::wstring totalSpace;
    std::wstring usedSpace;
    std::wstring freeSpace;
    std::wstring ratio;

private :
    std::wstring CreateFormattedSize(const std::wstring& rawSize) const;
};

class DfCommandParser : public AbstractOutputParser
{
public:
    DfCommandParser();

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual std::wstring GetMiniDescription();
    virtual std::wstring GetFullDescription();

    void GetDrives(std::vector<Drive>& drives) const;
    Drive GetFirstDrive() const;

private:
    bool FillDriveData(const std::vector<std::wstring>& lines);
    void TokenizeUsingWhitespaces(const std::wstring& buffer,
                                  std::vector<std::wstring>& tokens) const;
    bool IsDesirableDriveName(const std::wstring& name) const;

    std::wstring CreateResumedMiniDescription(const Drive& drive) const;
    std::wstring CreateDriveListDescription() const;
    std::wstring CreateFullDescription() const;

    std::vector<Drive> driveList;
};

#endif // DFCOMMANDPARSER_H

#ifndef DFCOMMANDPARSER_H
#define DFCOMMANDPARSER_H

#include "abstractdiskcommandparser.h"

class DfCommandParser : public AbstractDiskCommandParser
{
public:
    DfCommandParser();

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual std::wstring GetMiniDescription();
    virtual std::wstring GetFullDescription();

private:
    bool FillDriveData(const std::vector<std::wstring>& lines);
    void TokenizeUsingWhitespaces(const std::wstring& buffer,
                                  std::vector<std::wstring>& tokens) const;
    bool IsDesirableDriveName(const std::wstring& name) const;

    std::wstring CreateResumedMiniDescription(const LogicalDrive& drive) const;
    std::wstring CreateDriveListDescription() const;
    std::wstring CreateFullDescription() const;
};

#endif // DFCOMMANDPARSER_H

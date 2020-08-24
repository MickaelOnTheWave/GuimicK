#ifndef DFCOMMANDPARSER_H
#define DFCOMMANDPARSER_H

#include "abstractdiskcommandparser.h"

#include <sstream>

class DfCommandParser : public AbstractDiskCommandParser
{
public:
    DfCommandParser();

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual std::wstring GetMiniDescription();
    virtual std::wstring GetFullDescription();

private:
    void FillDriveData(const std::vector<std::wstring>& lines);
    void TokenizeUsingWhitespaces(const std::wstring& buffer,
                                  std::vector<std::wstring>& tokens) const;
    bool IsDesirableDriveName(const std::wstring& name) const;
    bool IsDfError(const std::wstring& name) const;

    std::wstring CreateResumedMiniDescription(const LogicalDrive& drive) const;
    std::wstring CreateResumedMiniDescription(const ErrorDrive& drive) const;

    std::wstring CreateDriveListDescription() const;
    std::wstring CreateFullDescription() const;

    void AddHealthyDrivesData(std::wstringstream& descriptionStream) const;
    void AddErrorDrivesData(std::wstringstream& descriptionStream) const;
};

#endif // DFCOMMANDPARSER_H

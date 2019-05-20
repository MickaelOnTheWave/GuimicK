#ifndef CHKDSKCOMMANDERRORPARSER_H
#define CHKDSKCOMMANDERRORPARSER_H

#include "abstractoutputparser.h"

class ChkdskCommandErrorParser
{
public:
    ChkdskCommandErrorParser();

    virtual bool ParseBuffer(const std::wstring& buffer) = 0;
    virtual std::wstring GetMiniDescription() = 0;
    virtual std::wstring GetFullDescription() = 0;

private:
    bool IsNoAdminRightsError(const std::wstring& buffer) const;
    bool IsInvalidDriveError(const std::wstring& buffer) const;

    std::wstring miniDescription;
    std::wstring fullDescription;
};

#endif // CHKDSKCOMMANDERRORPARSER_H

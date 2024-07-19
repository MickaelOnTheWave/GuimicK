#ifndef CHKDSKCOMMANDERRORPARSER_H
#define CHKDSKCOMMANDERRORPARSER_H

#include "abstractoutputparser.h"

class ChkdskCommandErrorParser : public AbstractOutputParser
{
public:
    ChkdskCommandErrorParser();

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual std::wstring GetMiniDescription();
    virtual std::wstring GetFullDescription();

private:
    bool IsNoAdminRightsError(const std::wstring& buffer) const;
    bool IsInvalidDriveError(const std::wstring& buffer) const;

    std::wstring miniDescription;
    std::wstring fullDescription;
};

#endif // CHKDSKCOMMANDERRORPARSER_H

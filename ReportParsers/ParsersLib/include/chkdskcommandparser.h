#ifndef CHKDSKCOMMANDPARSER_H
#define CHKDSKCOMMANDPARSER_H

#include "abstractoutputparser.h"

#include <vector>

class WindowsDrive
{
public:
    bool HasBadSectors() {return false;}

    std::wstring badSectors;
    std::wstring totalSpace;
    std::wstring freeSpace;
};

class ChkdskCommandParser : public AbstractOutputParser
{
public:
    ChkdskCommandParser();

    void ParseErrorBuffer(const std::wstring& buffer);

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual std::wstring GetMiniDescription();
    virtual std::wstring GetFullDescription();

private:
    void GetReportLines(const std::vector<std::wstring>& input,
                        std::vector<std::wstring> &output) const;

    bool CreateDriveDataFromReport(const std::vector<std::wstring>& reportLines);

    WindowsDrive drive;
};

#endif // CHKDSKCOMMANDPARSER_H

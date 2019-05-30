#ifndef CHKDSKCOMMANDPARSER_H
#define CHKDSKCOMMANDPARSER_H

#include "abstractdiskcommandparser.h"

class ChkdskCommandParser : public AbstractDiskCommandParser
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

};

#endif // CHKDSKCOMMANDPARSER_H

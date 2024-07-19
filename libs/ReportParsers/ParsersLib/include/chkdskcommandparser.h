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

    bool ParseSummarySection(const std::vector<std::wstring>& summary);

    std::wstring CreateOneDriveDescription() const;
    std::wstring CreateMultiDriveDescription() const;

    bool AllDrivesOk() const;
    bool AllDrivesFailing() const;

    std::wstring CreateAllDrivesOkMessage() const;
    std::wstring CreateAllDrivesFailingMessage() const;
    std::wstring CreateMixedResultsMessage() const;
};

#endif // CHKDSKCOMMANDPARSER_H

#ifndef SEVENZIPCOMMANDPARSER_H
#define SEVENZIPCOMMANDPARSER_H

#include "abstractfilebackupparser.h"

class SevenZipCommandParser : public AbstractFileBackupParser
{
public:
    SevenZipCommandParser();
    virtual ~SevenZipCommandParser();

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual void GetReport(FileBackupReport& _reportData);

private:
    void FillReportData(const std::vector<std::wstring>& lines);

    void UpdateReportData(std::vector<std::wstring>::const_iterator& it);
};

#endif // SEVENZIPCOMMANDPARSER_H

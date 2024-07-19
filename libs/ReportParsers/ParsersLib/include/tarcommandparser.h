#ifndef TARCOMMANDPARSER_H
#define TARCOMMANDPARSER_H

#include "abstractfilebackupparser.h"

class TarCommandParser : public AbstractFileBackupParser
{
public:
    TarCommandParser(const std::wstring& _tarCommand);
    virtual ~TarCommandParser();

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual void GetReport(FileBackupReport& _reportData);

private:
    void FillReportData(const std::vector<std::wstring>& lines);

    std::wstring tarCommand;
};

#endif // TARCOMMANDPARSER_H

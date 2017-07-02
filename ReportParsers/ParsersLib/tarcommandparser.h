#ifndef TARCOMMANDPARSER_H
#define TARCOMMANDPARSER_H

#include "abstractfilebackupparser.h"

class TarCommandParser : public AbstractFileBackupParser
{
public:
    TarCommandParser();
    virtual ~TarCommandParser();

    virtual bool ParseBuffer(const std::string& buffer);
    virtual void GetReport(FileBackupReport& _reportData);

private:
    void FillReportData(const std::vector<std::string>& lines);

};

#endif // TARCOMMANDPARSER_H

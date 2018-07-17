#ifndef TARCOMMANDPARSER_H
#define TARCOMMANDPARSER_H

#include "abstractfilebackupparser.h"

class TarCommandParser : public AbstractFileBackupParser
{
public:
    TarCommandParser(const std::string& _tarCommand);
    virtual ~TarCommandParser();

    virtual bool ParseBuffer(const std::string& buffer);
    virtual void GetReport(FileBackupReport& _reportData);

private:
    void FillReportData(const std::vector<std::string>& lines);

    std::string tarCommand;
};

#endif // TARCOMMANDPARSER_H

#ifndef SEVENZIPCOMMANDPARSER_H
#define SEVENZIPCOMMANDPARSER_H

#include "abstractfilebackupparser.h"

class SevenZipCommandParser : public AbstractFileBackupParser
{
public:
    SevenZipCommandParser();
    virtual ~SevenZipCommandParser();

    virtual bool ParseBuffer(const std::string& buffer);
    virtual void GetReport(FileBackupReport& _reportData);

private:
    void FillReportData(const std::vector<std::string>& lines);

};

#endif // SEVENZIPCOMMANDPARSER_H

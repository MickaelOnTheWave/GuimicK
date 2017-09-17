#ifndef GITPLUMBINGREPORTPARSER_H
#define GITPLUMBINGREPORTPARSER_H

#include "abstractfilebackupparser.h"

#include "filebackupreport.h"

class GitPlumbingReportParser : public AbstractFileBackupParser
{
public:
    GitPlumbingReportParser();
    virtual ~GitPlumbingReportParser();

    virtual bool ParseBuffer(const std::string& buffer);

    virtual void GetReport(FileBackupReport& reportData);

private:
    void FillReportData(const std::vector<std::string>& lines);
};

#endif // GITPLUMBINGREPORTPARSER_H

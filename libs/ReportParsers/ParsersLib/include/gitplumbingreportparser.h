#ifndef GITPLUMBINGREPORTPARSER_H
#define GITPLUMBINGREPORTPARSER_H

#include "abstractfilebackupparser.h"

#include "filebackupreport.h"

class GitPlumbingReportParser : public AbstractFileBackupParser
{
public:
    GitPlumbingReportParser();
    virtual ~GitPlumbingReportParser() = default;

    virtual bool ParseBuffer(const std::wstring& buffer);

    virtual void GetReport(FileBackupReport& reportData);

private:
    void FillReportData(const std::vector<std::wstring>& lines);
};

#endif // GITPLUMBINGREPORTPARSER_H

#ifndef GITCOMMITREPORTPARSER_H
#define GITCOMMITREPORTPARSER_H

#include "abstractfilebackupparser.h"

#include "filebackupreport.h"

class GitCommitReportParser : public AbstractFileBackupParser
{
public:
    GitCommitReportParser();
    virtual ~GitCommitReportParser();

    virtual bool ParseBuffer(const std::string& buffer);

    virtual void GetReport(FileBackupReport& reportData);

private:
    void FillReportData(const std::vector<std::string>& lines);
};

#endif // GITCOMMITREPORTPARSER_H

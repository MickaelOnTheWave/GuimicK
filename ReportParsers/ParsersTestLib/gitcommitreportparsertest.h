#ifndef GITCOMMITREPORTPARSERTEST_H
#define GITCOMMITREPORTPARSERTEST_H

#include "filebackupparserabstracttest.h"

class GitCommitReportParserTest : public FileBackupParserAbstractTest
{
    Q_OBJECT

public :
    GitCommitReportParserTest(const std::string& dataPrefix);

private Q_SLOTS:
    void testParse();

protected:
    virtual FileBackupReport* CreateReport();
    virtual AbstractFileBackupParser* CreateParser();

    virtual void PopulateTestData();

};

#endif // GITCOMMITREPORTPARSERTEST_H

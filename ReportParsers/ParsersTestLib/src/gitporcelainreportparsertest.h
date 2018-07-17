#ifndef GITPORCELAINREPORTPARSERTEST_H
#define GITPORCELAINREPORTPARSERTEST_H

#include "filebackupparserabstracttest.h"

class GitPorcelainReportParserTest : public FileBackupParserAbstractTest
{
    Q_OBJECT

public :
    GitPorcelainReportParserTest(const std::string& dataPrefix);

private Q_SLOTS:
    void testParse();

protected:
    virtual FileBackupReport* CreateReport();
    virtual AbstractFileBackupParser* CreateParser();

    virtual void PopulateTestData();
};

#endif // GITPORCELAINREPORTPARSERTEST_H

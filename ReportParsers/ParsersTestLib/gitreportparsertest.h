#ifndef GITREPORTPARSERTEST_H
#define GITREPORTPARSERTEST_H

#include "filebackupparserabstracttest.h"

#include <filetools.h>
#include <gitreportparser.h>

class GitReportParserTest : public FileBackupParserAbstractTest
{
    Q_OBJECT

public :
    GitReportParserTest(const std::string& dataPrefix);

private Q_SLOTS:
    void testParse();

protected:
    virtual FileBackupReport* CreateReport();
    virtual AbstractFileBackupParser* CreateParser();

    virtual void PopulateTestData();
};

#endif // GITREPORTPARSERTEST_H

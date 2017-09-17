#ifndef GITPLUMBINGREPORTPARSERTEST_H
#define GITPLUMBINGREPORTPARSERTEST_H

#include "filebackupparserabstracttest.h"

class GitPlumbingReportParserTest : public FileBackupParserAbstractTest
{
    Q_OBJECT

public :
    GitPlumbingReportParserTest(const std::string& dataPrefix);

private Q_SLOTS:
    void testParse();

protected:
    virtual FileBackupReport* CreateReport();
    virtual AbstractFileBackupParser* CreateParser();

    virtual void PopulateTestData();

};

#endif // GITPLUMBINGREPORTPARSERTEST_H

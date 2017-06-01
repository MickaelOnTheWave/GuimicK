#ifndef RSYNCCOMMANDPARSERTEST_H
#define RSYNCCOMMANDPARSERTEST_H

#include "filebackupparserabstracttest.h"

class RsyncCommandParserTest : public FileBackupParserAbstractTest
{
    Q_OBJECT

public:
    RsyncCommandParserTest(const std::string& dataPrefix);

private Q_SLOTS:
    void testParse();

protected:
    virtual FileBackupReport* CreateReport();
    virtual AbstractFileBackupParser* CreateParser();

    virtual void PopulateTestData();
};

#endif // RSYNCCOMMANDPARSERTEST_H

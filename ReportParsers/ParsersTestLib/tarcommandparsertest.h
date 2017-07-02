#ifndef TARCOMMANDPARSERTEST_H
#define TARCOMMANDPARSERTEST_H

#include "filebackupparserabstracttest.h"

class TarCommandParserTest : public FileBackupParserAbstractTest
{
    Q_OBJECT

public:
    TarCommandParserTest(const std::string& dataPrefix);

private Q_SLOTS:
    void testParse();

protected:
    virtual FileBackupReport* CreateReport();
    virtual AbstractFileBackupParser* CreateParser();

    virtual void PopulateTestData();

};

#endif // TARCOMMANDPARSERTEST_H

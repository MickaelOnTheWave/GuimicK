#include "tarcommandparsertest.h"

#include <QTest>
#include "tarcommandparser.h"

using namespace std;

const string suiteFolder = "TarCommand/";

TarCommandParserTest::TarCommandParserTest(const string &dataPrefix)
    : FileBackupParserAbstractTest(dataPrefix + suiteFolder)
{
}

void TarCommandParserTest::testParse()
{
    testParse_generic();
}

FileBackupReport *TarCommandParserTest::CreateReport()
{
    return new FileBackupReport();
}

AbstractFileBackupParser *TarCommandParserTest::CreateParser()
{
    return new TarCommandParser("/bin/tar");
}

void TarCommandParserTest::PopulateTestData()
{
    const QStringList fullFileList = {
        "ExtractDataFromGopro.sh",
        "encomenda cpb.pdf",
        "synology.pub",
        "TODOs (2016)"
    };

    QTest::newRow("Added") << "added4.txt" << fullFileList << QStringList() << QStringList();
    QTest::newRow("Empty") << "empty.txt" << QStringList() << QStringList() << QStringList();
    QTest::newRow("Warning") << "warningWithNoAdds.txt" << QStringList() << QStringList() << QStringList();
}

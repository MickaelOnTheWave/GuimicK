#include "rsynccommandparsertest.h"

#include <QTest>

#include "rsynccommandparser.h"

using namespace std;

const string suitePrefix = "RsyncCommand/";

RsyncCommandParserTest::RsyncCommandParserTest(const std::string& dataPrefix)
    : FileBackupParserAbstractTest(dataPrefix + suitePrefix)
{
}

void RsyncCommandParserTest::testParse()
{
    testParse_generic();
}

FileBackupReport *RsyncCommandParserTest::CreateReport()
{
    return new FileBackupReport();
}

AbstractFileBackupParser *RsyncCommandParserTest::CreateParser()
{
    return new RsyncCommandParser();
}

void RsyncCommandParserTest::PopulateTestData()
{
    QStringList fileListWith5;
    for (int i=0; i<5; ++i)
        fileListWith5 << QString("file%1").arg(i);

    QTest::newRow("Added 4 initial") << "add4"
                                     << QStringList({"wooo", "yaahaaaa", "yiiiii", "yoohoo"})
                                     << QStringList() << QStringList();
    QTest::newRow("Added 1 more") << "add1"
                                  << QStringList({"onemorefilenow"})
                                  << QStringList() << QStringList();
    QTest::newRow("Added folder") << "addfolder"
                                  << QStringList({"New Folder/", "New Folder/always_coke.jpg",
                                                  "New Folder/elections1.gif",
                                                  "New Folder/joie_bureau.gif",
                                                  "New Folder/manque_morceau.gif"
                                                 })
                                  << QStringList() << QStringList();
    QTest::newRow("Changed 5") << "change5" << QStringList()
                               << QStringList({
                                                  "onemorefilenow","wooo", "yaahaaaa",
                                                  "yiiiii", "yoohoo"
                                              })
                               << QStringList();
    QTest::newRow("Removed 4") << "remove4" << QStringList() << QStringList()
                               << QStringList({
                                                  "yiiiii", "yaahaaaa", "wooo",
                                                  "onemorefilenow",
                                              });
    QTest::newRow("Mixed changes") << "mixed"
                                   << QStringList({"On ne change pas","empty folder/"})
                                   << QStringList({"onemorefilenow"})
                                   << QStringList({
                                                      "yoohoo", "yiiiii", "wooo",
                                                      "New Folder/joie_bureau.gif",
                                                      "New Folder/elections1.gif"
                                                  });
}

#include "filebackupparserabstracttest.h"

#include <QTest>

using namespace std;

FileBackupParserAbstractTest::FileBackupParserAbstractTest()
{
}

void FileBackupParserAbstractTest::init()
{
    report = CreateReport();
}

void FileBackupParserAbstractTest::cleanup()
{
    delete report;
}

void FileBackupParserAbstractTest::testParse_InvalidFile()
{
    AbstractOutputParser* parser = CreateParser();
    bool returnValue = parser->ParseFile("inexistentFile");
    QCOMPARE(returnValue, false);
    delete parser;
}

void FileBackupParserAbstractTest::testParse_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QStringList>("added");
    QTest::addColumn<QStringList>("modified");
    QTest::addColumn<QStringList>("removed");

    PopulateTestData();
}

void FileBackupParserAbstractTest::testParse_generic()
{
    QFETCH(QString,     file);
    QFETCH(QStringList, added);
    QFETCH(QStringList, modified);
    QFETCH(QStringList, removed);

    GetReportDataFromCorrectFile(file.toStdString());
    CheckReportDataFiles(added, modified, removed);
}

void FileBackupParserAbstractTest::GetReportDataFromCorrectFile(const string &inputFile)
{
    AbstractFileBackupParser* parser = CreateParser();

    bool returnValue = parser->ParseFile(inputFile);
    Q_ASSERT(returnValue == true);

    parser->GetReport(*report);
    delete parser;
}

void FileBackupParserAbstractTest::CheckReportDataFiles(const QStringList &added,
                                                        const QStringList &modified,
                                                        const QStringList &removed)
{
    CheckListsAreEqual(report->added, added);
    CheckListsAreEqual(report->modified, modified);
    CheckListsAreEqual(report->removed, removed);
}

void FileBackupParserAbstractTest::CheckListsAreEqual(const vector<string> &actual,
                                                      const QStringList &expected)
{
    QCOMPARE(actual.size(), static_cast<unsigned long>(expected.size()));
    vector<string>::const_iterator it=actual.begin();
    for (; it!=actual.end(); ++it)
        QCOMPARE(expected.contains(QString((*it).c_str())), true);
}

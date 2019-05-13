#include "filebackupparserabstracttest.h"

#include <QTest>

using namespace std;

FileBackupParserAbstractTest::FileBackupParserAbstractTest(const wstring& dataPrefix)
    : QtTestSuite(dataPrefix)
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

    GetReportDataFromCorrectFile(GetDataFolder() + file.toStdWString());
    CheckReportDataFiles(added, modified, removed);
}

void FileBackupParserAbstractTest::GetReportDataFromCorrectFile(const wstring &inputFile)
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
   vector<wstring> listToCheck;

   report->GetAddedFiles(listToCheck);
   CheckListsAreEqual(listToCheck, added);

   report->GetModifiedFiles(listToCheck);
   CheckListsAreEqual(listToCheck, modified);

   report->GetRemovedFiles(listToCheck);
   CheckListsAreEqual(listToCheck, removed);
}

void FileBackupParserAbstractTest::CheckListsAreEqual(const vector<wstring>& actual,
                                                      const QStringList &expected)
{
    QCOMPARE(actual.size(), static_cast<unsigned long>(expected.size()));
    for (const auto& it : actual)
        QCOMPARE(expected.contains(QString::fromStdWString(it)), true);
}

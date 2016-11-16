#include "aptupgradeparsertest.h"

#include <QTest>

#include "aptgetupgradeparser.h"

using namespace std;

AptUpgradeParserTest::AptUpgradeParserTest()
{
}

void AptUpgradeParserTest::init()
{
    report.Clear();
}

void AptUpgradeParserTest::cleanup()
{
    report.Clear();
}

void AptUpgradeParserTest::testParse_InexistentFile()
{
    AptGetUpgradeParser parser;
    bool returnValue = parser.ParseFile("InexistentFile");
    QCOMPARE(returnValue, false);
}

void AptUpgradeParserTest::testParse_InvalidFileDoesNotCrash()
{
    AptGetUpgradeParser parser;
    bool returnValue = parser.ParseFile("image.jpeg");
    QCOMPARE(returnValue, false);
}

void AptUpgradeParserTest::testParse_data()
{
    // TODO : put sample data here
}

void AptUpgradeParserTest::testParse()
{
    QFETCH(QString,     file);
    QFETCH(QStringList, obsolete);
    QFETCH(QStringList, kept);
    QFETCH(QStringList, upgraded);
    QFETCH(QStringList, installed);
    QFETCH(QStringList, removed);
    QFETCH(QString,     updateFileSize);

    GetReportFromFile(file);
    CheckReportData(obsolete, kept, upgraded, installed, removed, updateFileSize);
}

void AptUpgradeParserTest::GetReportFromFile(const QString &file)
{
    AptGetUpgradeParser parser;
    bool returnValue = parser.ParseFile(file.toStdString());
    QCOMPARE(returnValue, true);

    parser.GetReport(report);
}

void AptUpgradeParserTest::CheckReportData(const QStringList& obsolete,
                                           const QStringList& kept,
                                           const QStringList& upgraded,
                                           const QStringList& installed,
                                           const QStringList& removed,
                                           const QString& updateFileSizeDescription)
{
    CheckListsAreEqual(report.installedPackages, obsolete);
    CheckListsAreEqual(report.keptPackages, kept);
    CheckListsAreEqual(report.obsoletePackages, upgraded);
    CheckListsAreEqual(report.removedPackages, installed);
    CheckListsAreEqual(report.upgradedPackages, removed);

    QCOMPARE(report.updateFileSize, updateFileSizeDescription.toStdString());
}

// TODO : refactor here to remove duplication with other test suites
void AptUpgradeParserTest::CheckListsAreEqual(const vector<string> &actual, const QStringList &expected)
{
    QCOMPARE(actual.size(), static_cast<unsigned long>(expected.size()));
    std::vector<std::string>::const_iterator it=actual.begin();
    for (; it!=actual.end(); ++it)
        QCOMPARE(expected.contains(QString((*it).c_str())), true);

}

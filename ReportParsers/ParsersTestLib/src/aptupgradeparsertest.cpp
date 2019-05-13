#include "aptupgradeparsertest.h"

#include <QTest>

#include "aptgetupgradeparser.h"

using namespace std;

AptUpgradeParserTest::AptUpgradeParserTest(const wstring &dataPrefix)
    : QtTestSuite(dataPrefix)
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
    bool returnValue = parser.ParseFile(L"InexistentFile");
    QCOMPARE(returnValue, false);
}

void AptUpgradeParserTest::testParse_InvalidFileDoesNotCrash()
{
    // TODO : improve parser implementation to check file for consistence.
    // So far it doesn't crash but has no way to separate invalid files from
    // empty reports.
    /*AptGetUpgradeParser parser;
    bool returnValue = parser.ParseFile("image.jpeg");
    QCOMPARE(returnValue, false);*/
}

void AptUpgradeParserTest::testParse_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QStringList>("obsolete");
    QTest::addColumn<QStringList>("kept");
    QTest::addColumn<QStringList>("upgraded");
    QTest::addColumn<QStringList>("installed");
    QTest::addColumn<QStringList>("removed");
    QTest::addColumn<QString>("updateFileSize");

    QTest::newRow("Example 1")
        << "apt1.txt"
        << QStringList({"libcuda1-331","nvidia-331-uvm","nvidia-340-uvm",
                        "nvidia-opencl-icd-331"})
        << QStringList({"icedtea-7-jre-jamvm","linux-generic","linux-headers-generic",
                        "linux-image-generic","linux-signed-generic","linux-signed-image-generic",
                        "openjdk-7-jre","openjdk-7-jre-headless"})
        << QStringList({"apt","apt-transport-https","apt-utils","curl","firefox",
                        "firefox-locale-en","firefox-locale-fr","firefox-locale-pt",
                        "libapt-inst1.5","libapt-pkg4.12","libcurl3","libcurl3-gnutls",
                        "libcurl4-openssl-dev","libmysqlclient18","libmysqlclient18:i386",
                        "mysql-client-core-5.5","mysql-common","mysql-server-core-5.5"})
        << QStringList()
        << QStringList()
        << QString("52,1 MB");
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

    GetReportFromFile(QString::fromStdWString(GetDataFolder()) + file);
    CheckReportData(obsolete, kept, upgraded, installed, removed, updateFileSize);
}

void AptUpgradeParserTest::GetReportFromFile(const QString &file)
{
    AptGetUpgradeParser parser;
    bool returnValue = parser.ParseFile(file.toStdWString());
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
    CheckListsAreEqual(report.installedPackages, installed);
    CheckListsAreEqual(report.keptPackages, kept);
    CheckListsAreEqual(report.obsoletePackages, obsolete);
    CheckListsAreEqual(report.removedPackages, removed);
    CheckListsAreEqual(report.upgradedPackages, upgraded);

    QCOMPARE(QString::fromStdWString(report.updateFileSize),
             updateFileSizeDescription);
}

// TODO : refactor here to remove duplication with other test suites
void AptUpgradeParserTest::CheckListsAreEqual(const std::vector<wstring>& actual,
                                              const QStringList &expected)
{
    QCOMPARE(actual.size(), static_cast<unsigned long>(expected.size()));
    vector<wstring>::const_iterator it=actual.begin();
    for (; it!=actual.end(); ++it)
        QCOMPARE(expected.contains(QString::fromStdWString((*it))), true);

}

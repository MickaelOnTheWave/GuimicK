#ifndef APTUPGRADEPARSERTEST_H
#define APTUPGRADEPARSERTEST_H

#include "qttestsuite.h"

#include "aptgetupgradeparser.h"

class AptUpgradeParserTest : public QtTestSuite
{
    Q_OBJECT

public:
    AptUpgradeParserTest(const std::string& dataPrefix);

private Q_SLOTS:
    void init();
    void cleanup();

    void testParse_InexistentFile();
    void testParse_InvalidFileDoesNotCrash();
    void testParse_data();
    void testParse();

private:
    void GetReportFromFile(const QString& file);
    void CheckReportData(const QStringList& obsolete,
                         const QStringList& kept,
                         const QStringList& upgraded,
                         const QStringList& installed,
                         const QStringList& removed,
                         const QString &updateFileSizeDescription);
    void CheckListsAreEqual(const std::vector<std::string>& actual,
                            const QStringList &expected);

    AptGetUpgradeReport report;
};

#endif // APTUPGRADEPARSERTEST_H

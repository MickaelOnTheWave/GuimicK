#include "gitplumbingreportparsertest.h"

#include <QTest>
#include "gitplumbingreportparser.h"

using namespace std;

const wstring suiteFolder = L"GitPlumbing/";

GitPlumbingReportParserTest::GitPlumbingReportParserTest(const wstring& dataPrefix)
    : FileBackupParserAbstractTest(dataPrefix + suiteFolder)
{
}

void GitPlumbingReportParserTest::testParse()
{
    testParse_generic();
}

FileBackupReport *GitPlumbingReportParserTest::CreateReport()
{
    return new FileBackupReport();
}

AbstractFileBackupParser *GitPlumbingReportParserTest::CreateParser()
{
    return new GitPlumbingReportParser();
}

void GitPlumbingReportParserTest::PopulateTestData()
{
    const QStringList fullFileList = {
        "6232100495809.sav",
        "GamerProfile.xml",
        "TODOs",
        "TODOs (2016)",
        "ex1_description.txt"
    };

    QTest::newRow("Modified 5") << "modified5.txt" << QStringList() << fullFileList << QStringList();
    QTest::newRow("Removed 5") << "removed5.txt" << QStringList() << QStringList() << fullFileList;
    QTest::newRow("All changes") << "allchanges.txt"
                                 << QStringList{"HomepageEsperancajpg"}
                                 << QStringList{"GamerProfile.xml", "ex1_description.txt"}
                                 << QStringList{"TODOs", "TODOs (2016)"};
}

#include "gitcommitreportparsertest.h"

#include <QTest>
#include "gitcommitreportparser.h"

using namespace std;

const string suiteFolder = "GitCommit/";

GitCommitReportParserTest::GitCommitReportParserTest(const std::string &dataPrefix)
    : FileBackupParserAbstractTest(dataPrefix + suiteFolder)
{
}

void GitCommitReportParserTest::testParse()
{
    testParse_generic();
}

FileBackupReport *GitCommitReportParserTest::CreateReport()
{
    return new FileBackupReport();
}

AbstractFileBackupParser *GitCommitReportParserTest::CreateParser()
{
    return new GitCommitReportParser();
}

void GitCommitReportParserTest::PopulateTestData()
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

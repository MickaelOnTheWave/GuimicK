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
    // TODO : create proper data

    QStringList fileListWith5;
    for (int i=0; i<5; ++i)
        fileListWith5 << QString("file%1").arg(i);

    QTest::newRow("Added 5") << "gitadd5.log" << fileListWith5 << QStringList() << QStringList();
    QTest::newRow("Changed 5") << "gitmod5.log" << QStringList() << fileListWith5 << QStringList();
    QTest::newRow("Removed 5") << "gitrm5.log" << QStringList() << QStringList() << fileListWith5;
    QTest::newRow("Mixed changes") << "gitallchanges.log"
                                   << QStringList({"alternate0","alternate1","alternate2"})
                                   << QStringList({"file0","file1","file2"})
                                   << QStringList({"file3","file4"});
}

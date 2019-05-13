#include "gitporcelainreportparsertest.h"

#include <QTest>
#include "gitporcelainreportparser.h"

using namespace std;

const wstring suiteFolder = L"GitPull/";

GitPorcelainReportParserTest::GitPorcelainReportParserTest(const wstring &dataPrefix)
    : FileBackupParserAbstractTest(dataPrefix + suiteFolder)
{
}

void GitPorcelainReportParserTest::testParse()
{
    testParse_generic();
}

FileBackupReport *GitPorcelainReportParserTest::CreateReport()
{
    return new FileBackupReport();
}

AbstractFileBackupParser *GitPorcelainReportParserTest::CreateParser()
{
    return new GitPorcelainReportParser();
}

void GitPorcelainReportParserTest::PopulateTestData()
{
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

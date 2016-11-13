#include "rsnapshotreportparsertest.h"

#include "rsnapshotreportparser.h"

using namespace std;

void RsnapshotReportParserTest::testParse()
{
    QFETCH(QString,     file);
    QFETCH(QStringList, added);
    QFETCH(QStringList, modified);
    QFETCH(QStringList, removed);

    testParse_generic();
    CheckReportByteData(added, modified, removed);
}

FileBackupReport *RsnapshotReportParserTest::CreateReport()
{
    return new RsnapshotReport();
}

AbstractFileBackupParser *RsnapshotReportParserTest::CreateParser()
{
    return new RSnapshotReportParser();
}

void RsnapshotReportParserTest::PopulateTestData()
{
    QTest::newRow("No changes") << "rsnapshotnochanges.log" <<
                                   QStringList() << QStringList() << QStringList();
    QTest::newRow("Added 3") << "rsnapshot3added.log"
                             << QStringList({"added0","added1","added2"})
                             << QStringList() << QStringList();
    QTest::newRow("Modified 3") << "rsnapshot3changed.log"
                                << QStringList()
                                << QStringList({"file0", "file1","file2"})
                                << QStringList();
    QTest::newRow("Removed 3") << "rsnapshot3removed.log"
                               << QStringList() << QStringList()
                               << QStringList({"file0", "file1","file2"});
    QTest::newRow("Mixed changes") << "rsnapshotmixedchanges.log"
                                   << QStringList({"myfirst add","my other add", "third add", "last add"})
                                   << QStringList({"file0","file1"})
                                   << QStringList({"file2"});
}

void RsnapshotReportParserTest::CheckReportByteData(const QStringList &added,
                                                    const QStringList &modified,
                                                    const QStringList &removed)
{
    long long bytesAdded = GetFilelistByteSize(added);
    long long bytesModified = GetFilelistByteSize(modified);
    long long bytesRemoved = GetFilelistByteSize(removed);

    RsnapshotReport* typedReport = dynamic_cast<RsnapshotReport*>(report);
    QCOMPARE(typedReport->bytesAdded, bytesAdded+bytesModified);
    QCOMPARE(typedReport->bytesRemoved, bytesRemoved+bytesModified);
}

long long RsnapshotReportParserTest::GetFilelistByteSize(const QStringList &filelist)
{
    long long bytes = 0;
    for (int i=0; i<filelist.size(); ++i)
        bytes += GetFileByteSize(filelist.at(i));
    return bytes;
}

long long RsnapshotReportParserTest::GetFileByteSize(const QString &filename)
{
    QFile file(filename);
    return file.size();
}



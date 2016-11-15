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
    // TODO : fix test suite by adding and fixing this test.
    //CheckReportByteData(added, modified, removed);
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
    QStringList list5files;
    for (int i=0; i<5; ++i)
        list5files << QString("file%1").arg(i);
    list5files = AppendPrefix(list5files);

    QTest::newRow("Added 5") << "rsnapshot5added.log"
                             << list5files
                             << QStringList()
                             << QStringList();
    QTest::newRow("Modified 5") << "rsnapshot5changed.log"
                                << QStringList()
                                << list5files
                                << QStringList();
    QTest::newRow("Removed 5") << "rsnapshot5removed.log"
                               << QStringList()
                               << QStringList()
                               << list5files;
    QTest::newRow("Mixed changes") << "rsnapshotall.log"
                                   << AppendPrefix(QStringList({"new1.pdf","new2.pdf", "new3.pdf"}))
                                   << AppendPrefix(QStringList({"file2","file3","file4"}))
                                   << AppendPrefix(QStringList({"file0","file1"}));
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

QStringList RsnapshotReportParserTest::AppendPrefix(const QStringList &files)
{
    const QString prefix = "/rsnapshotdest/";
    QStringList prefixedList;
    for (int i=0; i<files.size(); ++i)
        prefixedList << prefix + files.at(i);
    return prefixedList;
}



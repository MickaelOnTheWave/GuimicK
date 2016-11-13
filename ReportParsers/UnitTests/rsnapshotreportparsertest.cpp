#include "rsnapshotreportparsertest.h"

#include "rsnapshotreportparser.h"

using namespace std;

RsnapshotReportParserTest::RsnapshotReportParserTest()
{
}

void RsnapshotReportParserTest::init()
{
    report.Reset();
}

void RsnapshotReportParserTest::cleanup()
{
    report.Reset();
}

void RsnapshotReportParserTest::testParse_InvalidFile()
{
    RSnapshotReportParser parser;
    bool ok = parser.ParseFile("inexistentfile");
    QCOMPARE(ok, false);
}

void RsnapshotReportParserTest::testParse_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QStringList>("added");
    QTest::addColumn<QStringList>("modified");
    QTest::addColumn<QStringList>("removed");

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

void RsnapshotReportParserTest::testParse()
{
    QFETCH(QString,     file);
    QFETCH(QStringList, added);
    QFETCH(QStringList, modified);
    QFETCH(QStringList, removed);

    GetReportDataFromFile(file.toStdString());
    CheckReportDataFiles(added, modified, removed);
    CheckReportByteData(added, modified, removed);
}

void RsnapshotReportParserTest::GetReportDataFromFile(const std::string &file)
{
    RSnapshotReportParser parser;
    bool ok = parser.ParseFile(file);
    QCOMPARE(ok, true);
    parser.GetData(report);
}

void RsnapshotReportParserTest::CheckReportDataFiles(const QStringList &added,
                                                     const QStringList &modified,
                                                     const QStringList &removed)
{
    CheckListsAreEqual(report.addedList, added);
    CheckListsAreEqual(report.modifiedList, modified);
    CheckListsAreEqual(report.removedList, removed);
}

void RsnapshotReportParserTest::CheckListsAreEqual(const list<string> &actual,
                                                   const QStringList &expected)
{
    QCOMPARE(actual.size(), static_cast<unsigned long>(expected.size()));
    list<string>::const_iterator it=actual.begin();
    for (; it!=actual.end(); ++it)
        QCOMPARE(expected.contains(QString((*it).c_str())), true);
}

void RsnapshotReportParserTest::CheckReportByteData(long long added, long long removed)
{
    QCOMPARE(report.bytesAdded, added);
    QCOMPARE(report.bytesRemoved, removed);
}

void RsnapshotReportParserTest::CheckReportByteData(const QStringList &added,
                                                    const QStringList &modified,
                                                    const QStringList &removed)
{
    long long bytesAdded = GetFilelistByteSize(added);
    long long bytesModified = GetFilelistByteSize(modified);
    long long bytesRemoved = GetFilelistByteSize(removed);

    QCOMPARE(report.bytesAdded, bytesAdded+bytesModified);
    QCOMPARE(report.bytesRemoved, bytesRemoved+bytesModified);
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



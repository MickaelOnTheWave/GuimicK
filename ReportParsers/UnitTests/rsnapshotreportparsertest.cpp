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

void RsnapshotReportParserTest::testParse_Added5()
{
    QStringList addedFiles, modifiedList, removedList;
    for (int i=0; i<5; ++i)
        addedFiles.push_back(QString("file%1").arg(i));

    testParse_All("rsnapshot5added.log", addedFiles, modifiedList, removedList);
}

void RsnapshotReportParserTest::testParse_Changed5()
{
    QFAIL("TODO : implement this test");
}

void RsnapshotReportParserTest::testParse_Removed5()
{
    QFAIL("TODO : implement this test");
}

void RsnapshotReportParserTest::testParse_MixedChanges()
{
    QFAIL("TODO : implement this test");
}

void RsnapshotReportParserTest::testParse_All(const std::string &file,
                                              const QStringList &added,
                                              const QStringList &modified,
                                              const QStringList &removed)
{
    GetReportDataFromFile(file);
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

long long RsnapshotReportParserTest::GetFileByteSize(const QString &file)
{
    return 0;
}



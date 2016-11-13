#ifndef RSNAPSHOTREPORTPARSERTEST_H
#define RSNAPSHOTREPORTPARSERTEST_H

#include <QObject>
#include <QString>
#include <QtTest>

#include "rsnapshotreportparser.h"

class RsnapshotReportParserTest : public QObject
{
    Q_OBJECT

public:
    RsnapshotReportParserTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testParse_InvalidFile();
    void testParse_data();
    void testParse();

private:
    void GetReportDataFromFile(const std::string& file);
    void CheckReportDataFiles(const QStringList& added,
                              const QStringList& modified,
                              const QStringList& removed);
    void CheckListsAreEqual(const std::list<std::string>& actual, const QStringList& expected);
    void CheckReportByteData(long long added, long long removed);
    void CheckReportByteData(const QStringList& added,
                             const QStringList& modified,
                             const QStringList& removed);
    long long GetFilelistByteSize(const QStringList& filelist);
    long long GetFileByteSize(const QString& filename);


    BackupReportData report;
};

#endif // RSNAPSHOTREPORTPARSERTEST_H

#ifndef RSNAPSHOTREPORTPARSERTEST_H
#define RSNAPSHOTREPORTPARSERTEST_H

#include <QObject>
#include <QString>
#include <QtTest>

class RsnapshotReportParserTest : public QObject
{
    Q_OBJECT

public:
    RsnapshotReportParserTest();

private Q_SLOTS:
    void testParse_InvalidFile();
    void testParse_Added5();
    void testParse_Changed5();
    void testParse_Removed5();
    void testParse_MixedChanges();
};

#endif // RSNAPSHOTREPORTPARSERTEST_H

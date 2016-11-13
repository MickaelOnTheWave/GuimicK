#ifndef RSNAPSHOTREPORTPARSERTEST_H
#define RSNAPSHOTREPORTPARSERTEST_H

#include <QObject>
#include <QString>
#include <QtTest>

#include "filebackupparserabstracttest.h"
#include "rsnapshotreportparser.h"

class RsnapshotReportParserTest : public FileBackupParserAbstractTest
{
    Q_OBJECT

private Q_SLOTS:
    void testParse();

protected:
    virtual FileBackupReport* CreateReport();
    virtual AbstractFileBackupParser* CreateParser();

    virtual void PopulateTestData();

private:
    void CheckReportByteData(const QStringList& added,
                             const QStringList& modified,
                             const QStringList& removed);
    long long GetFilelistByteSize(const QStringList& filelist);
    long long GetFileByteSize(const QString& filename);
};

#endif // RSNAPSHOTREPORTPARSERTEST_H

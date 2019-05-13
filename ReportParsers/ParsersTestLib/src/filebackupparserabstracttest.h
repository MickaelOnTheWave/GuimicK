#ifndef FILEBACKUPPARSERABSTRACTTEST_H
#define FILEBACKUPPARSERABSTRACTTEST_H

#include "qttestsuite.h"

#include <QStringList>
#include <string>
#include <vector>

#include "abstractfilebackupparser.h"
#include "filebackupreport.h"

class FileBackupParserAbstractTest : public QtTestSuite
{
    Q_OBJECT
public:
    FileBackupParserAbstractTest(const std::wstring& dataPrefix);

private Q_SLOTS:
    void init();
    void cleanup();

    void testParse_data();

protected:
    virtual FileBackupReport* CreateReport() = 0;
    virtual AbstractFileBackupParser* CreateParser() = 0;

    virtual void PopulateTestData() = 0;

    void testParse_generic();

    void GetReportDataFromCorrectFile(const std::wstring& inputFile);
    void CheckReportDataFiles(const QStringList &added,
                              const QStringList &modified,
                              const QStringList &removed);
    void CheckListsAreEqual(const std::vector<std::wstring> &actual,
                            const QStringList &expected);

    FileBackupReport* report = nullptr;
};

#endif // FILEBACKUPPARSERABSTRACTTEST_H

#ifndef GITREPORTPARSERTEST_H
#define GITREPORTPARSERTEST_H

#include <QObject>

#include <filetools.h>
#include <gitreportparser.h>

class GitReportParserTest : public QObject
{
    Q_OBJECT

public:
    GitReportParserTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testParseFile_UnexistentFile();
    void testParseFile_UnknownFile();
    void testParseFile_Added5();
    void testParseFile_Changed5();
    void testParseFile_Removed5();
    void testParseFile_MixedChanges();

private:
    void TestWrongFile(const std::string& inputFile);
    void GetReportDataFromCorrectFile(const std::string& inputFile);
    void CheckReportDataFileCount(unsigned long added,
                                  unsigned long modified,
                                  unsigned long removed);

    GitReportData reportData;
    const std::string defaultOutputFile = "outputfile";
};

#endif // GITREPORTPARSERTEST_H

#include <QString>
#include <QtTest>

#include <string>

#include <filetools.h>
#include <gitreportparser.h>

class GitReportParserTest : public QObject
{
    Q_OBJECT

public:
    GitReportParserTest();

private Q_SLOTS:
    void testParseFile_UnexistentFile();
    void testParseFile_UnknownFile();
    void testParseFile_Added5();
    void testParseFile_Changed5();
    void testParseFile_Removed5();
    void testParseFile_MixedChanges();

private:
    void TestWrongFile(const std::string& inputFile);
    void GetReportDataFromCorrectFile(const std::string& inputFile,
                                      GitReportData& data);

    const std::string defaultOutputFile = "outputfile";
};

GitReportParserTest::GitReportParserTest()
{
}

void GitReportParserTest::testParseFile_UnexistentFile()
{
    TestWrongFile("nonexistentfile.ext");
}

void GitReportParserTest::testParseFile_UnknownFile()
{
    // TODO : improve error checking. Missing file or unknown
    // file return the same value so we can't say what we really tested.
    TestWrongFile("image.jpg");
}

void GitReportParserTest::testParseFile_Added5()
{
    GitReportData reportData;
    GetReportDataFromCorrectFile("gitadd5.log", reportData);

    QCOMPARE(reportData.addedFileList.size(), 5ul);
    QCOMPARE(reportData.modifiedFileList.size(), 0ul);
    QCOMPARE(reportData.removedFileList.size(), 0ul);

    for (int i=0; i<5; ++i)
        QCOMPARE(QString(reportData.addedFileList[i].c_str()), QString("file%1").arg(i));
}

void GitReportParserTest::testParseFile_Changed5()
{
    GitReportData reportData;
    GetReportDataFromCorrectFile("gitmod5.log", reportData);

    QCOMPARE(reportData.addedFileList.size(), 0ul);
    QCOMPARE(reportData.modifiedFileList.size(), 5ul);
    QCOMPARE(reportData.removedFileList.size(), 0ul);

    for (int i=0; i<5; ++i)
        QCOMPARE(QString(reportData.modifiedFileList[i].c_str()), QString("file%1").arg(i));
}

void GitReportParserTest::testParseFile_Removed5()
{
    GitReportData reportData;
    GetReportDataFromCorrectFile("gitrm5.log", reportData);

    QCOMPARE(reportData.addedFileList.size(), 0ul);
    QCOMPARE(reportData.modifiedFileList.size(), 0ul);
    QCOMPARE(reportData.removedFileList.size(), 5ul);

    for (int i=0; i<5; ++i)
        QCOMPARE(QString(reportData.removedFileList[i].c_str()), QString("file%1").arg(i));
}

void GitReportParserTest::testParseFile_MixedChanges()
{
    GitReportData reportData;
    GetReportDataFromCorrectFile("gitallchanges.log", reportData);

    QCOMPARE(reportData.addedFileList.size(), 3ul);
    QCOMPARE(reportData.modifiedFileList.size(), 3ul);
    QCOMPARE(reportData.removedFileList.size(), 2ul);

    for (int i=0; i<3; ++i)
        QCOMPARE(QString(reportData.addedFileList[i].c_str()), QString("alternate%1").arg(i));
    for (int i=0; i<3; ++i)
        QCOMPARE(QString(reportData.modifiedFileList[i].c_str()), QString("file%1").arg(i));
    for (int i=3; i<5; ++i)
        QCOMPARE(QString(reportData.removedFileList[i].c_str()), QString("file%1").arg(i));
}

void GitReportParserTest::TestWrongFile(const std::string &inputFile)
{
    std::remove(defaultOutputFile.c_str());

    GitReportParser parser;
    std::string description;
    bool returnValue = parser.ParseUsingFiles(inputFile, defaultOutputFile, description);
    Q_ASSERT(returnValue == false);
    Q_ASSERT(FileTools::FileExists(defaultOutputFile) == false);
    Q_ASSERT(description == "");
}

void GitReportParserTest::GetReportDataFromCorrectFile(const std::string &inputFile, GitReportData &data)
{
    std::remove(defaultOutputFile.c_str());

    GitReportParser parser;

    bool returnValue = parser.ParseFile(inputFile);
    Q_ASSERT(returnValue == true);

    parser.GetData(data);
}

QTEST_APPLESS_MAIN(GitReportParserTest)

#include "tst_unitteststest.moc"

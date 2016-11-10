#include "gitreportparsertest.h"

#include <QTest>

GitReportParserTest::GitReportParserTest()
{
}

void GitReportParserTest::init()
{
    reportData.Clear();
}

void GitReportParserTest::cleanup()
{
    reportData.Clear();
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
    GetReportDataFromCorrectFile("gitadd5.log");
    CheckReportDataFileCount(5,0,0);

    for (int i=0; i<5; ++i)
        QCOMPARE(QString(reportData.addedFileList[i].c_str()), QString("file%1").arg(i));
}

void GitReportParserTest::testParseFile_Changed5()
{
    GetReportDataFromCorrectFile("gitmod5.log");
    CheckReportDataFileCount(0,5,0);

    for (int i=0; i<5; ++i)
        QCOMPARE(QString(reportData.modifiedFileList[i].c_str()), QString("file%1").arg(i));
}

void GitReportParserTest::testParseFile_Removed5()
{
    GetReportDataFromCorrectFile("gitrm5.log");
    CheckReportDataFileCount(0,0,5);

    for (int i=0; i<5; ++i)
        QCOMPARE(QString(reportData.removedFileList[i].c_str()), QString("file%1").arg(i));
}

void GitReportParserTest::testParseFile_MixedChanges()
{
    GetReportDataFromCorrectFile("gitallchanges.log");
    CheckReportDataFileCount(3,3,2);

    for (int i=0; i<3; ++i)
        QCOMPARE(QString(reportData.addedFileList[i].c_str()), QString("alternate%1").arg(i));
    for (int i=0; i<3; ++i)
        QCOMPARE(QString(reportData.modifiedFileList[i].c_str()), QString("file%1").arg(i));
    QCOMPARE(QString(reportData.removedFileList[0].c_str()), QString("file3"));
    QCOMPARE(QString(reportData.removedFileList[1].c_str()), QString("file4"));
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

void GitReportParserTest::GetReportDataFromCorrectFile(const std::string &inputFile)
{
    std::remove(defaultOutputFile.c_str());

    GitReportParser parser;

    bool returnValue = parser.ParseFile(inputFile);
    Q_ASSERT(returnValue == true);

    parser.GetData(reportData);
}

void GitReportParserTest::CheckReportDataFileCount(unsigned long added,
                                                   unsigned long modified,
                                                   unsigned long removed)
{
    QCOMPARE(reportData.addedFileList.size(), added);
    QCOMPARE(reportData.modifiedFileList.size(), modified);
    QCOMPARE(reportData.removedFileList.size(), removed);
}

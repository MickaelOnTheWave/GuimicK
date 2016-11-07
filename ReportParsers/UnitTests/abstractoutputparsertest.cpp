#include "abstractoutputparsertest.h"

#include "mockparser.h"

AbstractOutputParserTest::AbstractOutputParserTest()
{

}

void AbstractOutputParserTest::test_ParseFile_InvalidFile()
{
    MockParser parser;
    bool result = parser.ParseFile("nonexistentfile");
    QCOMPARE(result, false);
}

void AbstractOutputParserTest::test_ParseFile_Ok()
{
    MockParser parser;
    bool result = parser.ParseFile("dummyDataFile");
    QCOMPARE(result, true);
    QCOMPARE(QString(parser.GetFileContent().c_str()), QString("multi\nline\ncontent\n"));
}

void AbstractOutputParserTest::test_WriteFullDescriptionToFile_Ok()
{
    const std::string outputFile("outputFile");
    const std::string contentToWrite("my\nmulti\nline\nwritten\ncontent");

    MockParser parser;
    parser.SetFullDescription(contentToWrite);
    parser.WriteFullDescriptionToFile(outputFile);

    QString readContent = GetContentFromFile(outputFile);
    QCOMPARE(readContent, QString(contentToWrite.c_str()));
}

QString AbstractOutputParserTest::GetContentFromFile(const std::string &file)
{
    QFile f(file.c_str());
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return QString("");

    QTextStream in(&f);
    return in.readAll();
}

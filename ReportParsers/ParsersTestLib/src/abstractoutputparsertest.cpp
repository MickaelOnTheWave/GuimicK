#include "abstractoutputparsertest.h"

#include "mockparser.h"

AbstractOutputParserTest::AbstractOutputParserTest(const std::wstring &dataPrefix)
    : QtTestSuite(dataPrefix)
{
}

void AbstractOutputParserTest::test_ParseFile_InvalidFile()
{
    MockParser parser;
    bool result = parser.ParseFile(GetDataFolder() + L"nonexistentfile");
    QCOMPARE(result, false);
}

void AbstractOutputParserTest::test_ParseFile_Ok()
{
    MockParser parser;
    bool result = parser.ParseFile(GetDataFolder() + L"dummyDataFile");
    QCOMPARE(result, true);
    QCOMPARE(QString::fromStdWString(parser.GetFileContent()),
             QString("multi\nline\ncontent\n"));
}

void AbstractOutputParserTest::test_WriteFullDescriptionToFile_Ok()
{
    const std::wstring outputFile(L"outputFile");
    const std::wstring contentToWrite(L"my\nmulti\nline\nwritten\ncontent");

    MockParser parser;
    parser.SetFullDescription(contentToWrite);
    parser.WriteFullDescriptionToFile(outputFile);

    QString readContent = GetContentFromFile(outputFile);
    QCOMPARE(readContent, QString::fromStdWString(contentToWrite));
}

QString AbstractOutputParserTest::GetContentFromFile(const std::wstring& file)
{
    QFile f(QString::fromStdWString(file));
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return QString("");

    QTextStream in(&f);
    return in.readAll();
}

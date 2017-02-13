#include "rsnapshoterroranalyzertest.h"

#include <QString>
#include <QTest>

#include "rsnapshoterroranalyzer.h"
#include "filetools.h"

RsnapshotErrorAnalyzerTest::RsnapshotErrorAnalyzerTest()
{
}

void RsnapshotErrorAnalyzerTest::init()
{

}

void RsnapshotErrorAnalyzerTest::cleanup()
{

}

void RsnapshotErrorAnalyzerTest::testOutput()
{
    QFETCH(QString, outputFile);
    QFETCH(bool, expectedResult);

    const std::string file = outputFile.toStdString();
    std::string commandOutput;
    FileTools::GetTextFileContent(file, commandOutput);

    RsnapshotErrorAnalyzer analyzer(commandOutput);
    bool result = analyzer.IsOutOfSpaceError();
    QCOMPARE(result, expectedResult);
}

void RsnapshotErrorAnalyzerTest::testOutput_data()
{
    QTest::addColumn<QString>("outputFile");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Normal") << "rsnapshotall.log" << false;
    QTest::newRow("Out of space") << "rsnapshot_nospaceerror.txt" << true;
}

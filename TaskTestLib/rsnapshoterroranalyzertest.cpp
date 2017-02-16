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
    QFETCH(bool, outOfSpaceError);
    QFETCH(bool, invalidFolderError);

    const std::string file = outputFile.toStdString();
    std::string commandOutput;
    FileTools::GetTextFileContent(file, commandOutput);

    RsnapshotErrorAnalyzer analyzer(commandOutput);
    QCOMPARE(analyzer.IsOutOfSpaceError(), outOfSpaceError);
    QCOMPARE(analyzer.IsInvalidFolderError(), invalidFolderError);
}

void RsnapshotErrorAnalyzerTest::testOutput_data()
{
    QTest::addColumn<QString>("outputFile");
    QTest::addColumn<bool>("outOfSpaceError");
    QTest::addColumn<bool>("invalidFolderError");

    QTest::newRow("Normal") << "rsnapshotall.log" << false << false;
    QTest::newRow("Out of space") << "rsnapshot_nospaceerror.txt" << true << false;
    QTest::newRow("Invalid folder") << "rsnapshot_invalidfolder.txt" << false << true;
}

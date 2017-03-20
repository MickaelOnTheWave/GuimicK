#include "parserstestsuites.h"

#include <QTest>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    //QObject* testSuite = new DfCommandParserTest();
    QObject* testSuite = new AbstractOutputParserTest("/home/mickael/Prog/TaskManager/ReportParsers/ParsersTestLib/data/");

    QTest::qExec(testSuite, argc, argv);
}


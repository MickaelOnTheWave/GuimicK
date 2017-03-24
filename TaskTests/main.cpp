#include <QTest>

#include "tasktestsuites.h"
#include "toolstestsuites.h"

using namespace std;

int main(int argc, char* argv[])
{
    //QObject* testSuite = new RsnapshotJobTest();
    //QObject* testSuite = new RsnapshotErrorAnalyzerTest();
    //QObject* testSuite = new TaskFeatureTest();
    //QObject* testSuite = new ConsoleJobTest();
    //QObject* testSuite = new GitJobTest();
    //QObject* testSuite = new UserConsoleJobTest();
    //QObject* testSuite = new SshConsoleJobTest();
    QObject* testSuite = new ConfigurationParserTest("/home/mickael/Prog/Tools/ToolsTestLib/data/");

    QTest::qExec(testSuite, argc, argv);

    delete testSuite;
}


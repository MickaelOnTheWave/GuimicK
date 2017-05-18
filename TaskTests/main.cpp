#include <QTest>

#include "parserstestsuites.h"
#include "tasktestsuites.h"
#include "toolstestsuites.h"

using namespace std;

const string taskLibDir = "/home/mickael/Prog/TaskManager/TaskTestLib/data/";
const string parsersDir = "/home/mickael/Prog/TaskManager/ReportParsers/ParsersTestLib/data/";

int main(int argc, char* argv[])
{
    //QObject* testSuite = new RsnapshotJobTest(taskLibDir);
    //QObject* testSuite = new RsnapshotErrorAnalyzerTest();
    //QObject* testSuite = new TaskFeatureTest();
    //QObject* testSuite = new ConsoleJobTest();
    //QObject* testSuite = new GitJobTest();
    //QObject* testSuite = new UserConsoleJobTest();
    //QObject* testSuite = new SshConsoleJobTest();
    //QObject* testSuite = new ConfigurationParserTest("/home/mickael/Prog/Tools/ToolsTestLib/data/");
    //QObject* testSuite = new FileToolsTest("/home/mickael/Prog/Tools/ToolsTestLib/data/");
    //QObject* testSuite = new ClientWorkManagerTest();
    //QObject* testSuite = new ConfigurationTest(taskLibDir);
    //QObject* testSuite = new GitFsBackupJobTest(taskLibDir);
    //QObject* testSuite = new GitCommitReportParserTest(parsersDir);
    QObject* testSuite = new CopyFsBackupJobTest(taskLibDir);

    QTest::qExec(testSuite, argc, argv);

    delete testSuite;
}


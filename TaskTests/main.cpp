#include <QTest>

#include "parserstestsuites.h"
#include "tasktestsuites.h"
#include "toolstestsuites.h"

using namespace std;

const string taskLibDir = "/home/mickael/Prog/TaskManager/TaskTestLib/data/";
const string parsersDir = "/home/mickael/Prog/TaskManager/ReportParsers/ParsersTestLib/data/";
const string toolsLibDir = "/home/mickael/Prog/Tools/ToolsTestLib/data/";

int main(int argc, char* argv[])
{
    //QObject* testSuite = new RsnapshotJobTest(taskLibDir);
    //QObject* testSuite = new RsnapshotErrorAnalyzerTest();
    QObject* testSuite = new TaskFeatureTest();
    //QObject* testSuite = new ConsoleJobTest();
    //QObject* testSuite = new GitJobTest();
    //QObject* testSuite = new UserConsoleJobTest();
    //QObject* testSuite = new SshConsoleJobTest();
    //QObject* testSuite = new ConfigurationParserTest(toolsLibDir);
    //QObject* testSuite = new FileToolsTest(toolsLibDir);
    //QObject* testSuite = new ClientWorkManagerTest();
    //QObject* testSuite = new ConfigurationTest(taskLibDir);
    //QObject* testSuite = new GitFsBackupJobTest(taskLibDir);
    //QObject* testSuite = new GitCommitReportParserTest(parsersDir);
    //QObject* testSuite = new RawCopyFsBackupJobTest(taskLibDir);
    //QObject* testSuite = new RsyncCopyFsBackupJobTest(taskLibDir);
    //QObject* testSuite = new RsyncCommandParserTest(parsersDir);

    QTest::qExec(testSuite, argc, argv);

    delete testSuite;
}


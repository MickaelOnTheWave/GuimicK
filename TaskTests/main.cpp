#include <QTest>
#include <unistd.h>

#include "filetools.h"
#include "parserstestsuites.h"
#include "tasktestsuites.h"
#include "toolstestsuites.h"

using namespace std;

const string taskLibDir  = "/home/mickael/Prog/TaskManager/TaskTestLib/data/";
const string parsersDir  = "/home/mickael/Prog/TaskManager/ReportParsers/ParsersTestLib/data/";
const string toolsLibDir = "/home/mickael/Prog/Tools/ToolsTestLib/data/";
const string errorDir    = "/home/mickael/Prog/TaskManager/TaskTestLib/data/errors/";

const string testingFolder = "TaskManagerTestSuites/";

int main(int argc, char* argv[])
{
    if (FileTools::FolderExists(testingFolder) == false)
        FileTools::CreateFolder(testingFolder);
    chdir(testingFolder.c_str());

    //QObject* testSuite = new RsnapshotJobTest(taskLibDir);
    //QObject* testSuite = new RsnapshotErrorAnalyzerTest(taskLibDir);
    //QObject* testSuite = new TaskFeatureTest(taskLibDir, errorDir);
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
    QObject* testSuite = new ZipAndCopyFsBackupJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new TarCommandParserTest(parsersDir);

    QTest::qExec(testSuite, argc, argv);

    delete testSuite;
}


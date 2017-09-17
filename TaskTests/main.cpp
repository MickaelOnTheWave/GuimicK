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

    //QObject* testSuite = new RsnapshotJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new RsnapshotErrorAnalyzerTest(taskLibDir);
    //QObject* testSuite = new TaskFeatureTest(taskLibDir, errorDir);
    //QObject* testSuite = new ConsoleJobTest();
    //QObject* testSuite = new GitJobTest();
    //QObject* testSuite = new UserConsoleJobTest();
    QObject* testSuite = new SshConsoleJobTest();
    //QObject* testSuite = new ConfigurationParserTest(toolsLibDir);
    //QObject* testSuite = new FileToolsTest(toolsLibDir);
    //QObject* testSuite = new ClientWorkManagerTest();
    //QObject* testSuite = new ConfigurationTest(taskLibDir);
    //QObject* testSuite = new GitFsBackupJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new GitCommitReportParserTest(parsersDir);
    //QObject* testSuite = new RawCopyFsBackupJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new RsyncCopyFsBackupJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new RsyncCommandParserTest(parsersDir);
    //QObject* testSuite = new ZipAndCopyFsBackupJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new TarCommandParserTest(parsersDir);
    //QObject* testSuite = new GitFsBackupJobConfigurationTest();
    //QObject* testSuite = new RsnapshotBackupJobConfigurationTest();
    //QObject* testSuite = new WakeJobConfigurationTest();
    //QObject* testSuite = new ChangeScreensaverJobConfigurationTest();
    //QObject* testSuite = new ClamAvJobConfigurationTest();
    //QObject* testSuite = new ShutdownJobConfigurationTest();
    //QObject* testSuite = new UserConsoleJobConfigurationTest();
    //QObject* testSuite = new SshConsoleJobConfigurationTest();
    //QObject* testSuite = new GitBackupJobConfigurationTest();
    //QObject* testSuite = new DiskSpaceCheckJobConfigurationTest();
    //QObject* testSuite = new RawCopyFsBackupJobConfigurationTest();
    //QObject* testSuite = new RsyncCopyFsBackupJobConfigurationTest();
    //QObject* testSuite = new ZipAndCopyFsBackupJobConfigurationTest();

    QTest::qExec(testSuite, argc, argv);

    delete testSuite;
}


#include <QTest>

#include "filetools.h"
#include "pathtools.h"
#include "parserstestsuites.h"
#include "tasktestsuites.h"
#include "toolstestsuites.h"

using namespace std;

const wstring progRoot    = L"/home/mickael/Prog";
const wstring taskLibDir  = progRoot + L"/TaskManager/TaskTestLib/data/";
const wstring parsersDir  = progRoot + L"/TaskManager/ReportParsers/ParsersTestLib/data/";
const wstring toolsLibDir = progRoot + L"/Tools/ToolsTestLib/data/";
const wstring errorDir    = progRoot + L"/TaskManager/TaskTestLib/data/errors/";

const wstring testingFolder = L"TaskManagerTestSuites/";

int main(int argc, char* argv[])
{
    if (FileTools::FolderExists(testingFolder) == false)
        FileTools::CreateFolder(testingFolder);
    PathTools::ChangeCurrentDir(testingFolder);

    //QObject* testSuite = new RsnapshotJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new RsnapshotErrorAnalyzerTest(taskLibDir);
    //QObject* testSuite = new TaskFeatureTest(taskLibDir, errorDir);
    //QObject* testSuite = new ConsoleJobTest();
    //QObject* testSuite = new GitJobTest();
    //QObject* testSuite = new UserConsoleJobTest();
    //QObject* testSuite = new SshConsoleJobTest();
    //QObject* testSuite = new ClientWorkManagerTest();
    //QObject* testSuite = new ServerConfigurationTest(taskLibDir);
    //QObject* testSuite = new GitFsBackupJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new GitPlumbingReportParserTest(parsersDir);
    //QObject* testSuite = new RawCopyFsBackupJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new RsyncCopyFsBackupJobTest(taskLibDir, errorDir);
    //QObject* testSuite = new RsyncCommandParserTest(parsersDir);
    QObject* testSuite = new ZipAndCopyFsBackupJobTest(taskLibDir, errorDir);
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
    //QObject* testSuite = new ToolsTest(toolsLibDir);
    //QObject* testSuite = new GitPorcelainReportParserTest(parsersDir);
    //QObject* testSuite = new RemoteJobsRunnerTest();
    //QObject* testSuite = new RemoteJobsRunnerConfigurationTest();
    //QObject* testSuite = new WakeJobTest();
    //QObject* testSuite = new TarIncrementalBackupJobTest();
    //QObject* testSuite = new TarIncrementalBackupJobConfigurationTest();
    //QObject* testSuite = new AbstractOutputParserTest(parsersDir);

    QTest::qExec(testSuite, argc, argv);

    delete testSuite;
}


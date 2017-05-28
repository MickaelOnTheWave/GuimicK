#include "tasktestlib.h"

#include "tasktestsuites.h"

void GetTaskLibTests(std::vector<QObject*>& tests, const std::string& dataPrefix)
{
    tests.push_back(new RsnapshotErrorAnalyzerTest());
    tests.push_back(new RsnapshotJobTest(dataPrefix));
    tests.push_back(new GitJobTest());
    tests.push_back(new TaskFeatureTest());
    tests.push_back(new ConsoleJobTest());
    tests.push_back(new UserConsoleJobTest());
    tests.push_back(new SshConsoleJobTest());
    tests.push_back(new ClientWorkManagerTest());
    tests.push_back(new ConfigurationTest(dataPrefix));
    tests.push_back(new GitFsBackupJobTest(dataPrefix));
    tests.push_back(new RawCopyFsBackupJobTest(dataPrefix));
    tests.push_back(new RsyncCopyFsBackupJobTest(dataPrefix));
}



#include "tasktestlib.h"

#include "tasktestsuites.h"

void GetTaskLibTests(std::vector<QObject*>& tests,
                     const std::string& dataPrefix,
                     const std::string& errorPrefix)
{
    tests.push_back(new RsnapshotErrorAnalyzerTest(dataPrefix));
    tests.push_back(new RsnapshotJobTest(dataPrefix, errorPrefix));
    tests.push_back(new GitJobTest());
    tests.push_back(new TaskFeatureTest(dataPrefix, errorPrefix));
    tests.push_back(new ConsoleJobTest());
    tests.push_back(new UserConsoleJobTest());
    tests.push_back(new SshConsoleJobTest());
    tests.push_back(new ClientWorkManagerTest());
    tests.push_back(new ConfigurationTest(dataPrefix));
    tests.push_back(new GitFsBackupJobTest(dataPrefix, errorPrefix));
    tests.push_back(new RawCopyFsBackupJobTest(dataPrefix, errorPrefix));
    tests.push_back(new RsyncCopyFsBackupJobTest(dataPrefix, errorPrefix));
    tests.push_back(new ZipAndCopyFsBackupJobTest(dataPrefix, errorPrefix));
    tests.push_back(new GitFsBackupJobConfigurationTest());
}



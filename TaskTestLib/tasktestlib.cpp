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
    tests.push_back(new RsnapshotBackupJobConfigurationTest());
    tests.push_back(new WakeJobConfigurationTest());
    tests.push_back(new ChangeScreensaverJobConfigurationTest());
    tests.push_back(new ClamAvJobConfigurationTest());
    tests.push_back(new ShutdownJobConfigurationTest());
    tests.push_back(new UserConsoleJobConfigurationTest());
    tests.push_back(new SshConsoleJobConfigurationTest());
    tests.push_back(new GitBackupJobConfigurationTest());
    tests.push_back(new DiskSpaceCheckJobConfigurationTest());
    tests.push_back(new RawCopyFsBackupJobConfigurationTest());
    tests.push_back(new RsyncCopyFsBackupJobConfigurationTest());
    tests.push_back(new ZipAndCopyFsBackupJobConfigurationTest());
}



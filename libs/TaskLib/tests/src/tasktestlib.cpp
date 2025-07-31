#include "tasktestlib.h"

#include "tasktestsuites.h"

void GetTaskLibTests(std::vector<QObject*>& tests,
                     const std::wstring& dataPrefix,
                     const std::wstring& errorPrefix)
{
   // Disabled for now, for some reason the ssh password is no more asked and thus it fails
   // tests.
   // TODO : enable and fix this once Windows features are done (ssh is more advanced).
   tests.push_back(new RsnapshotErrorAnalyzerTest(dataPrefix));
   tests.push_back(new RsnapshotJobTest(dataPrefix, errorPrefix));
   tests.push_back(new GitJobTest());
   //tests.push_back(new TaskFeatureTest(dataPrefix, errorPrefix));
   /*tests.push_back(new ConsoleJobTest());
   tests.push_back(new UserConsoleJobTest());
   tests.push_back(new SshConsoleJobTest());
   tests.push_back(new ClientWorkManagerTest());*/
   //tests.push_back(new ServerConfigurationTest(dataPrefix));
   //tests.push_back(new GitFsBackupJobTest(dataPrefix, errorPrefix));
   //tests.push_back(new RawCopyFsBackupJobTest(dataPrefix, errorPrefix));
   //tests.push_back(new RsyncCopyFsBackupJobTest(dataPrefix, errorPrefix));
   tests.push_back(new ZipAndCopyFsBackupJobTest(dataPrefix, errorPrefix));
   tests.push_back(new GitFsBackupJobConfigurationTest());
   tests.push_back(new RsnapshotBackupJobConfigurationTest());
   tests.push_back(new WakeJobConfigurationTest());
   tests.push_back(new ClamAvJobConfigurationTest());
   tests.push_back(new ShutdownJobConfigurationTest());
   tests.push_back(new UserConsoleJobConfigurationTest());
   tests.push_back(new SshConsoleJobConfigurationTest());
   tests.push_back(new GitBackupJobConfigurationTest());
   tests.push_back(new DiskSpaceCheckJobConfigurationTest());
   tests.push_back(new RawCopyFsBackupJobConfigurationTest());
   tests.push_back(new RsyncCopyFsBackupJobConfigurationTest());
   tests.push_back(new ZipAndCopyFsBackupJobConfigurationTest());
   tests.push_back(new RemoteJobsRunnerConfigurationTest());
   tests.push_back(new WakeJobTest());
   //tests.push_back(new TarIncrementalBackupJobTest());
   tests.push_back(new TarIncrementalBackupJobConfigurationTest());
}



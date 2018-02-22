#include "tarincrementalbackupjobtest.h"

#include <QTest>

#include "tarincrementalbackup.h"

using namespace std;

const string suitePrefix = "GitFsBackup/";

TarIncrementalBackupJobTest::TarIncrementalBackupJobTest(
      const string& dataPrefix, const string& errorPrefix)
   : AbstractFsBackupJobTest(dataPrefix + suitePrefix, errorPrefix)
{
}

void TarIncrementalBackupJobTest::testRunBackup_data()
{
   LoadExternalDataSamples(false);
}

AbstractBackupJob* TarIncrementalBackupJobTest::CreateNewJob()
{
   return new TarIncrementalBackup();
}

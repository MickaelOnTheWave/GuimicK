#include "copyfsbackupjobconfigurationstests.h"
#include "copyfsbackupjobconfigurations.h"

AbstractJobConfiguration *RawCopyFsBackupJobConfigurationTest::CreateNewConfiguration() const
{
    return new OsCopyFsBackupJobConfiguration();
}

AbstractJobConfiguration *RsyncCopyFsBackupJobConfigurationTest::CreateNewConfiguration() const
{
    return new RsyncCopyFsBackupJobConfiguration();
}

AbstractJobConfiguration *ZipAndCopyFsBackupJobConfigurationTest::CreateNewConfiguration() const
{
    return new ZipAndCopyFsBackupJobConfiguration();
}



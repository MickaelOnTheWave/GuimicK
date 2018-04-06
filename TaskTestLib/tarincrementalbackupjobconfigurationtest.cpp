#include "tarincrementalbackupjobconfigurationtest.h"
#include "tarincrementalbackupjobconfiguration.h"

AbstractJobConfiguration *TarIncrementalBackupJobConfigurationTest::CreateNewConfiguration() const
{
    return new TarIncrementalBackupJobConfiguration();
}

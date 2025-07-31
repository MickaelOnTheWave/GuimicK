#include "gitfsbackupjobconfigurationtest.h"

#include "gitfsbackupjobconfiguration.h"

AbstractJobConfiguration *GitFsBackupJobConfigurationTest::CreateNewConfiguration() const
{
    return new GitFsBackupJobConfiguration();
}

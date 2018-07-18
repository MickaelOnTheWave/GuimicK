#include "abstractfsbackupjobconfiguration.h"

AbstractFsBackupJobConfiguration::AbstractFsBackupJobConfiguration(const std::string &tag)
    : AbstractBackupJobConfiguration(tag)
{
}

std::string AbstractFsBackupJobConfiguration::GetBackupItemName() const
{
    return std::string("Folder");
}

#include "tarincrementalbackupjobconfiguration.h"
#include "tarincrementalbackup.h"

TarIncrementalBackupJobConfiguration::TarIncrementalBackupJobConfiguration()
   : AbstractBackupJobConfiguration("TarIncrementalBackup")
{
}

bool TarIncrementalBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<TarIncrementalBackup*>(job) != NULL);
}

AbstractJob* TarIncrementalBackupJobConfiguration::CreateJob()
{
   return new TarIncrementalBackup();
}

std::string TarIncrementalBackupJobConfiguration::GetBackupItemName() const
{
   return std::string("Folder");
}

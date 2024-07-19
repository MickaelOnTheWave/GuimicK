#include "tarincrementalbackupjobconfiguration.h"
#include "tarincrementalbackup.h"

TarIncrementalBackupJobConfiguration::TarIncrementalBackupJobConfiguration()
   : AbstractBackupJobConfiguration(L"TarIncrementalBackup")
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

std::wstring TarIncrementalBackupJobConfiguration::GetBackupItemName() const
{
   return L"Folder";
}

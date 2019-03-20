#include "tarbackupjobconfiguration.h"
#include "tarbackupjob.h"

//-------------------------------------------------------------------------//

TarBackupJobConfiguration::TarBackupJobConfiguration()
    : AbstractFsBackupJobConfiguration(L"TarBackup")
{
}

bool TarBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<TarBackupJob*>(job) != NULL);
}

AbstractJob *TarBackupJobConfiguration::CreateJob()
{
    return new TarBackupJob();
}

//-------------------------------------------------------------------------//

#include "sevenzipbackupjobconfiguration.h"
#include "sevenzipbackupjob.h"

//-------------------------------------------------------------------------//

SevenZipBackupJobConfiguration::SevenZipBackupJobConfiguration()
    : AbstractFsBackupJobConfiguration(L"7zipBackup")
{
}

bool SevenZipBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<SevenZipBackupJob*>(job) != NULL);
}

AbstractJob *SevenZipBackupJobConfiguration::CreateJob()
{
    return new SevenZipBackupJob();
}

//-------------------------------------------------------------------------//

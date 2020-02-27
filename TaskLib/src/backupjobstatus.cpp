#include "backupjobstatus.h"

using namespace std;

BackupJobStatus::BackupJobStatus()
   : JobStatus(), backupReport(NULL)
{
}

BackupJobStatus::BackupJobStatus(const int _code, const wstring& _description)
   : JobStatus(_code, _description), backupReport(NULL)
{
}

BackupJobStatus::BackupJobStatus(FileBackupReport* _backupReport)
   : JobStatus(), backupReport(_backupReport)
{
}

BackupJobStatus::BackupJobStatus(const JobStatus& status, FileBackupReport* _backupReport)
   : JobStatus(status), backupReport(_backupReport)
{
}

BackupJobStatus::BackupJobStatus(const BackupJobStatus& other)
   : JobStatus(other)
{
   if (other.backupReport != NULL)
      backupReport = new FileBackupReport(*other.backupReport);
   else
      backupReport = NULL;
}

BackupJobStatus::~BackupJobStatus()
{
   delete backupReport;
}

FileBackupReport* BackupJobStatus::GetFileReport() const
{
   return backupReport;
}

void BackupJobStatus::SetFileReport(FileBackupReport* fileReport)
{
   backupReport = fileReport;
}

#ifndef BACKUPJOBSTATUS_H
#define BACKUPJOBSTATUS_H

#include "filebackupreport.h"
#include "jobstatus.h"

class BackupJobStatus : public JobStatus
{
public:
   BackupJobStatus();
   BackupJobStatus(const int _code, const std::wstring& _description = L"");
   BackupJobStatus(FileBackupReport* _backupReport);
   BackupJobStatus(const JobStatus& status, FileBackupReport* _backupReport);
   BackupJobStatus(const BackupJobStatus& other);

   virtual ~BackupJobStatus();

   FileBackupReport* GetFileReport() const;
   void SetFileReport(FileBackupReport* fileReport);


private:
   FileBackupReport* backupReport;
};

#endif // JOBSTATUS_H

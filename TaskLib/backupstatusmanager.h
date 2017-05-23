#ifndef BACKUPSTATUSMANAGER_H
#define BACKUPSTATUSMANAGER_H

#include "abstractbackupjob.h"
#include "jobdebuginformationmanager.h"
#include "jobstatus.h"

class BackupStatusManager
{
public :
    BackupStatusManager();
    BackupStatusManager(const BackupStatusManager& other);

    void SetDebugManager(JobDebugInformationManager* _debugManager);
    void SetJoinReports(const bool value);

    JobStatus *CreateGlobalStatus(const AbstractBackupJob::ResultCollection& results,
                                  const AbstractBackupJob::BackupCollection& backups);

private:
    JobStatus* CreateSingleStatus();
    JobStatus* CreateAllOkStatus();
    JobStatus* CreateMixedStatus();

    JobStatus* CreateJoinedStatus();
    JobStatus* CreateSeparatedStatus();

    bool AreAllStatusesEqual(const int expectedCode);
    std::string CreateStatusesDescription();
    std::string CreateFoldersMiniDescription();
    int ComputeSuccessCount() const;

    std::string BuildRepositoryHeader(const std::string &name);
    std::string BuildFooter();

    const AbstractBackupJob::ResultCollection* resultCollection;
    const AbstractBackupJob::BackupCollection* backupCollection;
    JobDebugInformationManager* debugManager;
    bool joinReports;
};

#endif // BACKUPSTATUSMANAGER_H

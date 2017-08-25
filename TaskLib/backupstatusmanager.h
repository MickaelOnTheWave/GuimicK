#ifndef BACKUPSTATUSMANAGER_H
#define BACKUPSTATUSMANAGER_H

#include "abstractbackupjob.h"
#include "jobdebuginformationmanager.h"
#include "jobstatus.h"

class BackupStatusManager
{
public :
    BackupStatusManager(const std::string& _attachmentName = "attachment.txt");
    BackupStatusManager(const BackupStatusManager& other);

    void SetDebugManager(JobDebugInformationManager* _debugManager);
    bool GetJoinReports() const;
    void SetJoinReports(const bool value);
    void SetAttachmentName(const std::string& name);
    void SetItemBackupMessage(const std::string& message);

    JobStatus *CreateGlobalStatus(const AbstractBackupJob::ResultCollection& results,
                                  const AbstractBackupJob::BackupCollection& backups);

private:
    JobStatus* CreateSingleStatus();
    JobStatus* CreateAllOkStatus();

    JobStatus* CreateJoinedStatus();
    JobStatus* CreateSeparatedStatus(const int code);

    bool AreAllStatusesEqual(const int expectedCode);
    std::string CreateStatusesDescription();
    std::string CreateRepositoriesMiniDescription();
    int ComputeSuccessCount() const;

    std::string BuildRepositoryHeader(const std::string &name);
    std::string BuildFooter();

    std::string GetCorrectMiniDescription(
            const AbstractBackupJob::ResultEntry& result) const;

    FileBackupReport* CreateGlobalReport() const;

    const AbstractBackupJob::ResultCollection* resultCollection;
    const AbstractBackupJob::BackupCollection* backupCollection;
    JobDebugInformationManager* debugManager;
    bool joinReports;
    std::string attachmentName;
    std::string itemBackupMessage;
};

#endif // BACKUPSTATUSMANAGER_H

#ifndef BACKUPSTATUSMANAGER_H
#define BACKUPSTATUSMANAGER_H

#include "abstractbackupjob.h"
#include "jobdebuginformationmanager.h"
#include "jobstatus.h"

class BackupStatusManager
{
public :
    BackupStatusManager(const std::wstring& _attachmentName = L"attachment.txt");
    BackupStatusManager(const BackupStatusManager& other);

    void SetDebugManager(JobDebugInformationManager* _debugManager);
    bool GetJoinReports() const;
    void SetJoinReports(const bool value);
    void SetAttachmentName(const std::wstring& name);
    void SetItemBackupMessage(const std::wstring& message);

    JobStatus *CreateGlobalStatus(const AbstractBackupJob::ResultCollection& results,
                                  const AbstractBackupJob::BackupCollection& backups);

private:
    JobStatus* CreateSingleStatus();
    JobStatus* CreateAllOkStatus();

    JobStatus* CreateJoinedStatus();
    JobStatus* CreateSeparatedStatus(const int code);

    bool AreAllStatusesEqual(const int expectedCode);
    std::wstring CreateStatusesDescription();
    std::wstring CreateRepositoriesMiniDescription();
    int ComputeSuccessCount() const;

    static std::wstring BuildRepositoryHeader(const std::wstring &name);
    static std::wstring BuildFooter();

    std::wstring GetCorrectMiniDescription(
            const AbstractBackupJob::ResultEntry& result) const;

    FileBackupReport* CreateGlobalReport() const;

    std::wstring BuildRepositoryOkReport(FileBackupReport* report) const;

    std::wstring BuildRepositoryErrorReport(JobStatus* status) const;

    const AbstractBackupJob::ResultCollection* resultCollection;
    const AbstractBackupJob::BackupCollection* backupCollection;
    JobDebugInformationManager* debugManager;
    bool joinReports;
    std::wstring attachmentName;
    std::wstring itemBackupMessage;
};

#endif // BACKUPSTATUSMANAGER_H

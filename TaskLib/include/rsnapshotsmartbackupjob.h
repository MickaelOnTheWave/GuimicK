#ifndef RSNAPSHOTSMARTBACKUPJOB_H
#define RSNAPSHOTSMARTBACKUPJOB_H

#include "abstractbackupjob.h"

#include "rsnapshotrawbackupjob.h"

/**
 * This job implements the Rsnapshot backup using the standard Backup job interface.
 * It uses in the background the raw Rsnapshot backup.
 */
class RsnapshotSmartBackupJob : public AbstractBackupJob
{
public:
    RsnapshotSmartBackupJob();
    RsnapshotSmartBackupJob(const RsnapshotSmartBackupJob& other);
    virtual ~RsnapshotSmartBackupJob();

    virtual AbstractJob* Clone();

    virtual JobStatus* Run();

    virtual std::wstring GetTypeName() const;

    virtual void SetRepository(const std::wstring& value);
    virtual void AddFolder(const std::wstring& source, const std::wstring& destination);

    virtual JobStatus* RestoreBackupFromClient( const BackupRestoreParameters& parameters,
                                                const BackupRestoreTarget& target);

    std::wstring GetTemplateConfigurationFile() const;
    void SetTemplateConfigurationFile(const std::wstring& value);

    std::wstring GetTemporaryFile() const;
    void SetTemporaryFile(const std::wstring& value);

    bool GetWaitBeforeRun() const;
    void SetWaitBeforeRun(const bool value);

    int GetMaxBackupCount() const;
    void SetMaxBackupCount(const int value);

protected:
    virtual void RunRepositoryBackup(const std::wstring& source,
                                     const std::wstring& destination,
                                     ResultCollection& results);

private:
    virtual JobStatus* RestoreBackupFromServer(const std::wstring &source, const std::wstring &destination);

    virtual std::wstring CreateBackupSourcePath(const std::wstring& backupTag) const;

    JobStatus* RunConfiguredBackupJob();

    JobStatus* RunRawCopy(const std::wstring &source, const std::wstring &destination);

    std::wstring AppendTrailingSlash(const std::wstring value) const;

    std::wstring CreateConfiguration() const;

    RsnapshotRawBackupJob* CreateRawJob(const std::wstring& configuration) const;

    std::wstring BuildFinalPath(const std::wstring& inputPath) const;

    std::wstring templateConfigurationFile;
    std::wstring temporaryFile;
    bool waitBeforeRun;
    int maxBackupCount;
};

#endif // RSNAPSHOTSMARTBACKUPJOB_H

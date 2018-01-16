#ifndef RSNAPSHOTSMARTBACKUPJOB_H
#define RSNAPSHOTSMARTBACKUPJOB_H

#include "abstractbackupjob.h"

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

    virtual std::string GetTypeName() const;

    std::string GetTemplateConfigurationFile() const;
    void SetTemplateConfigurationFile(const std::string& value);

    std::string GetTemporaryFile() const;
    void SetTemporaryFile(const std::string& value);

    bool GetWaitBeforeRun() const;
    void SetWaitBeforeRun(const bool value);

    int GetMaxBackupCount() const;
    void SetMaxBackupCount(const int value);

protected:
    virtual void RunRepositoryBackup(const std::string& source,
                                     const std::string& destination,
                                     ResultCollection& results);

private:
    virtual JobStatus* RestoreBackup(const std::string &source, const std::string &destination);
    JobStatus* RunConfiguredBackupJob();

    void RemoveFile(const std::string& file); // TODO : remove this and put it in FileTools

    std::string templateConfigurationFile;
    std::string temporaryFile;
    BackupCollection dataToBackup;
    bool waitBeforeRun;
    int maxBackupCount;
};

#endif // RSNAPSHOTSMARTBACKUPJOB_H

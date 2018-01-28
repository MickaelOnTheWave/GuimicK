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

    virtual std::string GetTypeName() const;

    virtual void SetRepository(const std::string& value);
    virtual void AddFolder(const std::string& source, const std::string& destination);

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

    virtual std::string CreateBackupSourcePath(const std::string& backupTag) const;

    JobStatus* RunConfiguredBackupJob();

    JobStatus* RunRawCopy(const std::string &source, const std::string &destination);

    std::string AppendTrailingSlash(const std::string value) const;

    std::string CreateConfiguration() const;

    RsnapshotRawBackupJob* CreateRawJob(const std::string& configuration) const;


    std::string templateConfigurationFile;
    std::string temporaryFile;
    bool waitBeforeRun;
    int maxBackupCount;
};

#endif // RSNAPSHOTSMARTBACKUPJOB_H

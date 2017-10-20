#ifndef RSNAPSHOTRAWBACKUPJOB_H
#define RSNAPSHOTRAWBACKUPJOB_H

#include "abstractjob.h"
#include "consolejob.h"
#include <rsnapshotreportparser.h>

class RsnapshotRawBackupJob : public AbstractJob
{
public:
    RsnapshotRawBackupJob(const std::string& _backupRepositoryPath = "",
                          const std::string& _rsnapshotConfFile = "");
    RsnapshotRawBackupJob(const RsnapshotRawBackupJob& other);
    virtual ~RsnapshotRawBackupJob();

    virtual AbstractJob* Clone();
    virtual bool IsInitialized();

    virtual JobStatus* Run();

    std::string GetRepository() const;
    void SetRepository(const std::string& value);

    std::string GetConfigurationFile() const;
    void SetConfigurationFile(const std::string& value);

    /**
     * On automated testing, if there is no wait after
     * Rsnapshot run, data is not retrieved normally.
     */
    void SetWaitBeforeRun(const bool value);

private:
    JobStatus* RunBackup();
    JobStatus* RunReportCreation();
    JobStatus* RunRecursiveListReportCreation();
    JobStatus* RunRsnapshotDiffReportCreation();
    JobStatus* CreateParsedReportStatus();

    bool IsFirstBackup() const;

    ConsoleJob* CreateBackupCommandJob();
    ConsoleJob* CreateReportCommandJob();

    std::string configurationFile;
    std::string repository;
    bool waitBeforeRun;
    std::string reportJobOutput;
};

#endif // RSNAPSHOTRAWBACKUPJOB_H

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

    virtual std::string GetName();
    virtual AbstractJob* Clone();
    virtual bool InitializeFromClient(Client *client);
    virtual bool IsInitialized();

    virtual JobStatus* Run();

    void SetRepository(const std::string& value);
    void SetConfigurationFile(const std::string& value);

    /**
     * On automated testing, if there is no wait after
     * Rsnapshot run, data is not retrieved normally.
     */
    void SetWaitAfterRun(const bool value);

private:
    JobStatus* RunBackup();
    JobStatus* RunReportCreation();
    JobStatus* CreateParsedReportStatus(const std::string &output);

    ConsoleJob* CreateBackupCommandJob();
    ConsoleJob* CreateReportCommandJob();

    std::string configurationFile;
    std::string repository;
    bool waitAfterRun;
};

#endif // RSNAPSHOTRAWBACKUPJOB_H

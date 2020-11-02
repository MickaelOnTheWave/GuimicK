#ifndef RSNAPSHOTRAWBACKUPJOB_H
#define RSNAPSHOTRAWBACKUPJOB_H

#include "abstractjob.h"
#include "consolejob.h"
#include <rsnapshotreportparser.h>

class RsnapshotRawBackupJob : public AbstractJob
{
public:
    RsnapshotRawBackupJob(const std::wstring& _backupRepositoryPath = L"",
                          const std::wstring& _rsnapshotConfFile = L"");
    RsnapshotRawBackupJob(const RsnapshotRawBackupJob& other);
    virtual ~RsnapshotRawBackupJob();

    virtual AbstractJob* Clone();
    virtual bool IsInitialized();

    virtual JobStatus* Run();

    std::wstring GetRepository() const;
    void SetRepository(const std::wstring& value);

    std::wstring GetConfigurationFile() const;
    void SetConfigurationFile(const std::wstring& value);

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
    ConsoleJob* CreateChildCommandJob(
       const std::wstring& command,
       const std::wstring& parameters
    );

    std::wstring configurationFile;
    std::wstring repository;
    bool waitBeforeRun;
    std::wstring reportJobOutput;
};

#endif // RSNAPSHOTRAWBACKUPJOB_H

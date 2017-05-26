#ifndef COPYFSBACKUPJOB_H
#define COPYFSBACKUPJOB_H

#include "abstractbackupjob.h"
#include "backupstatusmanager.h"
#include "consolejob.h"
#include "filebackupreport.h"
#include "jobdebuginformationmanager.h"

class CopyFsBackupJob : public AbstractBackupJob
{
public:
    CopyFsBackupJob();
    CopyFsBackupJob(const CopyFsBackupJob& other);

    virtual std::string GetName();
    virtual AbstractJob* Clone();

    /**
     * @return command return code.
     */
    int RunOnParameters(const std::string &source,
                         const std::string &destination);

    void SetJoinAllBackups(const bool value);

protected:
    virtual void RunRepositoryBackup(const std::string& source,
                                    const std::string& destination,
                                     ResultCollection& results);
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);

private:
    void RunRawCopy(const std::string& source,
                    const std::string& destination,
                    ResultCollection& results);
    void RunRsyncCopy(  const std::string& source,
                        const std::string& destination,
                        ResultCollection& results);

    void PrepareRawCopyCommand(const std::string &source, const std::string &destination,
                               ConsoleJob& commandJob);
    void PrepareRsyncCommand(const std::string &source, const std::string &destination,
                             ConsoleJob& commandJob);

    void CreateRawCopyReport(const std::string& destination, ResultCollection& results);
    void CreateRsyncReport(const std::string &output, ResultCollection& results);
    void CreateCopyErrorReport(const std::string &message, ResultCollection& results);

    BackupStatusManager statusManager;
};

#endif // COPYFSBACKUPJOB_H

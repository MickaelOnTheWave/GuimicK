#ifndef ABSTRACTCOPYFSBACKUPJOB_H
#define ABSTRACTCOPYFSBACKUPJOB_H

#include "abstractbackupjob.h"
#include "backupstatusmanager.h"
#include "consolejob.h"

class AbstractCopyFsBackupJob : public AbstractBackupJob
{
public:
    AbstractCopyFsBackupJob(const std::string& debugFilename);
    AbstractCopyFsBackupJob(const AbstractCopyFsBackupJob& other);
    virtual ~AbstractCopyFsBackupJob();

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

    virtual void PrepareCopyCommand(const std::string &source,
                                    const std::string &destination,
                                    ConsoleJob& commandJob) = 0;

    virtual void CreateReport(const std::string& destination,
                              const std::string& output, ResultCollection& results) = 0;

private:
    void RunCopy(const std::string& source, const std::string& destination,
                 ResultCollection& results);

    void CreateCopyErrorReport(const std::string &message, ResultCollection& results);

    BackupStatusManager statusManager;
};

#endif // ABSTRACTCOPYFSBACKUPJOB_H

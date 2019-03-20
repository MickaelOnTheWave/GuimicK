#ifndef ABSTRACTCOPYFSBACKUPJOB_H
#define ABSTRACTCOPYFSBACKUPJOB_H

#include "abstractbackupjob.h"
#include "backupstatusmanager.h"
#include "consolejob.h"

class AbstractCopyFsBackupJob : public AbstractBackupJob
{
public:
    AbstractCopyFsBackupJob(const std::wstring& _title);
    AbstractCopyFsBackupJob(const AbstractCopyFsBackupJob& other);
    virtual ~AbstractCopyFsBackupJob();

    /**
     * @return command return code.
     */
    int RunOnParameters(const std::wstring &source,
                        const std::wstring &destination);

protected:
    virtual void RunRepositoryBackup(const std::wstring& source,
                                    const std::wstring& destination,
                                     ResultCollection& results);

    virtual void PrepareCopyCommand(const std::wstring &source,
                                    const std::wstring &destination,
                                    ConsoleJob& commandJob) = 0;

    virtual void CreateReport(const std::wstring& destination,
                              const std::wstring& output, ResultCollection& results) = 0;

private:
   virtual JobStatus* RestoreBackupFromServer(const std::wstring& source, const std::wstring& destination);

    void RunCopy(const std::wstring& source, const std::wstring& destination,
                 ResultCollection& results);

    void CreateCopyErrorReport(const std::wstring &message, ResultCollection& results);
};

#endif // ABSTRACTCOPYFSBACKUPJOB_H

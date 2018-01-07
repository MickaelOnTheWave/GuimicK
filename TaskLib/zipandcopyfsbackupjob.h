#ifndef ZIPANDCOPYFSBACKUPJOB_H
#define ZIPANDCOPYFSBACKUPJOB_H

#include "abstractbackupjob.h"
#include "backupstatusmanager.h"
#include "AbstractConsoleJob.h"

class ZipAndCopyFsBackupJob : public AbstractBackupJob
{
public:
    ZipAndCopyFsBackupJob();
    ZipAndCopyFsBackupJob(const ZipAndCopyFsBackupJob& other);

    virtual AbstractJob* Clone();

    virtual std::string GetTypeName() const;

    static bool Restore(const std::string& backupFile,
                        const std::string& destination);

    std::string GetLocalDestination() const;
    void SetLocalDestination(const std::string& value);

protected:
    virtual void RunRepositoryBackup(const std::string& source,
                                     const std::string& destination,
                                     ResultCollection& results);

private:
    bool CreateBackupArchive(const std::string& folderToBackup,
                             const std::string& archiveName,
                             ResultCollection& results);
    bool RemovePreviousArchive(const std::string& destination,
                               ResultCollection& results);

    bool CopyBackupArchiveToDestination(const std::string& destination,
                                        ResultCollection& results);

    bool CleanBackupArchiveFromSource(ResultCollection& results);

    void AddStatusToResults(ResultCollection& results,
                            const int code, const std::string& message);

    AbstractConsoleJob *CreateBackupConsoleJob(const std::string& parameters);

    std::string localDestination;
};

#endif // ZIPANDCOPYFSBACKUPJOB_H

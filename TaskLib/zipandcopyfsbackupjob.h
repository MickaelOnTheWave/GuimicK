#ifndef ZIPANDCOPYFSBACKUPJOB_H
#define ZIPANDCOPYFSBACKUPJOB_H

#include "abstractbackupjob.h"

class ZipAndCopyFsBackupJob : public AbstractBackupJob
{
public:
    ZipAndCopyFsBackupJob();
    ZipAndCopyFsBackupJob(const ZipAndCopyFsBackupJob& other);

    virtual std::string GetName();
    virtual AbstractJob* Clone();

    static bool Restore(const std::string& backupFile,
                        const std::string& destination);

protected:
    virtual void RunRepositoryBackup(const std::string& source,
                                     const std::string& destination,
                                     ResultCollection& results);
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);

private:
    bool CreateBackupArchive(const std::string& folderToBackup,
                             ResultCollection& results);
    bool PrepareDestination(const std::string& destination,
                             ResultCollection& results);
    bool MoveBackupArchiveToDestination(const std::string& destination,
                                        ResultCollection& results);
};

#endif // ZIPANDCOPYFSBACKUPJOB_H

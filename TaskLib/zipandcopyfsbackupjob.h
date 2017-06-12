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

protected:
    virtual void RunRepositoryBackup(const std::string& source,
                                     const std::string& destination,
                                     ResultCollection& results);
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);
};

#endif // ZIPANDCOPYFSBACKUPJOB_H

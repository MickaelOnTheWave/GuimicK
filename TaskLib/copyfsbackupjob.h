#ifndef COPYFSBACKUPJOB_H
#define COPYFSBACKUPJOB_H

#include "abstractbackupjob.h"

class CopyFsBackupJob : public AbstractBackupJob
{
public:
    CopyFsBackupJob();
    CopyFsBackupJob(const CopyFsBackupJob& other);

    virtual std::string GetName();
    virtual AbstractJob* Clone();
    virtual JobStatus* Run();
};

#endif // COPYFSBACKUPJOB_H

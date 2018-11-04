#ifndef RAWCOPYFSBACKUPJOB_H
#define RAWCOPYFSBACKUPJOB_H

#include "abstractcopyfsbackupjob.h"

class AbstractRawCopyFsBackupJob : public AbstractCopyFsBackupJob
{
public:
    AbstractRawCopyFsBackupJob();
    AbstractRawCopyFsBackupJob(const AbstractCopyFsBackupJob& other);

    virtual std::string GetTypeName() const;
};

#endif // RAWCOPYFSBACKUPJOB_H

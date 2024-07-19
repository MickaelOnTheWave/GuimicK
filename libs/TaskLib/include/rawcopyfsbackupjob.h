#ifndef RAWCOPYFSBACKUPJOB_H
#define RAWCOPYFSBACKUPJOB_H

#include "abstractcopyfsbackupjob.h"

class AbstractRawCopyFsBackupJob : public AbstractCopyFsBackupJob
{
public:
    AbstractRawCopyFsBackupJob();
    AbstractRawCopyFsBackupJob(const AbstractCopyFsBackupJob& other);

    virtual std::wstring GetTypeName() const;
};

#endif // RAWCOPYFSBACKUPJOB_H

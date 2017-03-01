#ifndef RSNAPSHOTSMARTBACKUPJOB_H
#define RSNAPSHOTSMARTBACKUPJOB_H

#include "rsnapshotbackupjob.h"

/**
 * This job differs from its parent job in that it removes its configuration
 * file when destroyed, considering that it was a temporary construction.
 */
class RsnapshotSmartBackupJob : public RsnapshotBackupJob
{
public:
    RsnapshotSmartBackupJob(const std::string& _backupRepositoryPath,
                            const std::string& _rsnapshotConfFile);
    ~RsnapshotSmartBackupJob();
};

#endif // RSNAPSHOTSMARTBACKUPJOB_H

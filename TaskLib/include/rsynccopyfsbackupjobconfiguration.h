#ifndef RSYNCCOPYFSBACKUPJOBCONFIGURATION_H
#define RSYNCCOPYFSBACKUPJOBCONFIGURATION_H

#include "abstractfsbackupjobconfiguration.h"

class RsyncCopyFsBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    RsyncCopyFsBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};

#endif // RSYNCCOPYFSBACKUPJOBCONFIGURATION_H

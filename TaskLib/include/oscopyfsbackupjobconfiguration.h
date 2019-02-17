#ifndef OSCOPYFSBACKUPJOBCONFIGURATION_H
#define OSCOPYFSBACKUPJOBCONFIGURATION_H

#include "abstractfsbackupjobconfiguration.h"

class OsCopyFsBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    OsCopyFsBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};

#endif // OSCOPYFSBACKUPJOBCONFIGURATION_H

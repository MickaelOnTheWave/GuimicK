#ifndef TarBackupJobConfiguration_H
#define TarBackupJobConfiguration_H

#include "abstractfsbackupjobconfiguration.h"

class TarBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    TarBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};


#endif // TarBackupJobConfiguration_H

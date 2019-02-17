#ifndef SevenZipBackupJobConfiguration_H
#define SevenZipBackupJobConfiguration_H

#include "abstractfsbackupjobconfiguration.h"

class SevenZipBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    SevenZipBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};


#endif // SevenZipBackupJobConfiguration_H

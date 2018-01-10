#ifndef COPYFSBACKUPJOBCONFIGURATIONS_H
#define COPYFSBACKUPJOBCONFIGURATIONS_H

#include "abstractfsbackupjobconfiguration.h"

class RawCopyFsBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    RawCopyFsBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};

class RsyncCopyFsBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    RsyncCopyFsBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};

class ZipAndCopyFsBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    ZipAndCopyFsBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};


#endif // COPYFSBACKUPJOBCONFIGURATIONS_H

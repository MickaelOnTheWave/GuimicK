#ifndef COPYFSBACKUPJOBCONFIGURATIONS_H
#define COPYFSBACKUPJOBCONFIGURATIONS_H

#include "abstractfsbackupjobconfiguration.h"

class RawCopyFsBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    RawCopyFsBackupJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
};

class RsyncCopyFsBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    RsyncCopyFsBackupJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
};

class ZipAndCopyFsBackupJobConfiguration : public AbstractFsBackupJobConfiguration
{
public:
    ZipAndCopyFsBackupJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
};


#endif // COPYFSBACKUPJOBCONFIGURATIONS_H

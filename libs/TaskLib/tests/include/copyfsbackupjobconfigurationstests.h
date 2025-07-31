#ifndef COPYFSBACKUPJOBCONFIGURATIONSTESTS_H
#define COPYFSBACKUPJOBCONFIGURATIONSTESTS_H

#include "abstractbackupjobconfigurationtest.h"

class RawCopyFsBackupJobConfigurationTest : public AbstractBackupJobConfigurationTest
{
    Q_OBJECT
public:
    RawCopyFsBackupJobConfigurationTest() = default;
    virtual ~RawCopyFsBackupJobConfigurationTest() = default;

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;
};

class RsyncCopyFsBackupJobConfigurationTest : public AbstractBackupJobConfigurationTest
{
    Q_OBJECT
public:
    RsyncCopyFsBackupJobConfigurationTest() = default;
    virtual ~RsyncCopyFsBackupJobConfigurationTest() = default;

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;
};

class ZipAndCopyFsBackupJobConfigurationTest : public AbstractBackupJobConfigurationTest
{
    Q_OBJECT
public:
    ZipAndCopyFsBackupJobConfigurationTest() = default;
    virtual ~ZipAndCopyFsBackupJobConfigurationTest() = default;

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;
};

#endif // COPYFSBACKUPJOBCONFIGURATIONSTESTS_H

#ifndef GITFSBACKUPJOBCONFIGURATIONTEST_H
#define GITFSBACKUPJOBCONFIGURATIONTEST_H

#include "abstractbackupjobconfigurationtest.h"

class GitFsBackupJobConfigurationTest : public AbstractBackupJobConfigurationTest
{
    Q_OBJECT

public:
    GitFsBackupJobConfigurationTest() = default;
    virtual ~GitFsBackupJobConfigurationTest() = default;

private:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;
};

#endif // GITFSBACKUPJOBCONFIGURATIONTEST_H

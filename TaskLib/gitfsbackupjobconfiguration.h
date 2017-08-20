#ifndef GITFSBACKUPJOBCONFIGURATION_H
#define GITFSBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"

class GitFsBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    GitFsBackupJobConfiguration();

protected:
    virtual AbstractJob *CreateJob();

private:
    virtual std::string GetBackupItemName() const;

};

#endif // GITFSBACKUPJOBCONFIGURATION_H

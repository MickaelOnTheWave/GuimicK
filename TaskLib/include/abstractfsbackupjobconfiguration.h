#ifndef COPYFSBACKUPJOBCONFIGURATION_H
#define COPYFSBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"

class AbstractFsBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    AbstractFsBackupJobConfiguration(const std::string& tag);

    virtual std::string GetBackupItemName() const;
};

#endif // COPYFSBACKUPJOBCONFIGURATION_H

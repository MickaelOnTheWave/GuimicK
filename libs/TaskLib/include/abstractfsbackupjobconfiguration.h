#ifndef COPYFSBACKUPJOBCONFIGURATION_H
#define COPYFSBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"

class AbstractFsBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    AbstractFsBackupJobConfiguration(const std::wstring& tag);

    virtual std::wstring GetBackupItemName() const;
};

#endif // COPYFSBACKUPJOBCONFIGURATION_H

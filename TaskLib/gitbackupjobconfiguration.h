#ifndef GITBACKUPJOBCONFIGURATION_H
#define GITBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"

class GitBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    GitBackupJobConfiguration();

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                                            ConfigurationObject *confObject,
                                            std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);
    virtual void FillKnownSubObjects(std::vector<std::string>& objects);
};

#endif // GITBACKUPJOBCONFIGURATION_H

#ifndef GITFSBACKUPJOBCONFIGURATION_H
#define GITFSBACKUPJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class GitFsBackupJobConfiguration : public AbstractJobConfiguration
{
public:
    GitFsBackupJobConfiguration();

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                                            ConfigurationObject *confObject,
                                            std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);
    virtual void FillKnownSubObjects(std::vector<std::string>& objects);

};

#endif // GITFSBACKUPJOBCONFIGURATION_H

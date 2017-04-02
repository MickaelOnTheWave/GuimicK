#ifndef RSNAPSHOTBACKUPJOBCONFIGURATION_H
#define RSNAPSHOTBACKUPJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"
#include "rsnapshotbackupjob.h"

class RsnapshotBackupJobConfiguration : public AbstractJobConfiguration
{
public:
    RsnapshotBackupJobConfiguration();

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(ConfigurationObject *confObject,
                                                       std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);
    virtual void FillKnownSubObjects(std::vector<std::string>& objects);

private:
    RsnapshotBackupJob *CreateRsnapshotBackupJobFromCreator(
                            ConfigurationObject *object,
                            const std::string &repository) const;
};

#endif // RSNAPSHOTBACKUPJOBCONFIGURATION_H

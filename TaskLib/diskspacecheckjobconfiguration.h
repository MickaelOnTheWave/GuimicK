#ifndef DISKSPACECHECKJOBCONFIGURATION_H
#define DISKSPACECHECKJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class DiskSpaceCheckJobConfiguration : public AbstractJobConfiguration
{
public:
    DiskSpaceCheckJobConfiguration();

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                                ConfigurationObject *confObject,
                                std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // DISKSPACECHECKJOBCONFIGURATION_H

#ifndef SHUTDOWNJOBCONFIGURATION_H
#define SHUTDOWNJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class ShutdownJobConfiguration : public AbstractJobConfiguration
{
public:
    ShutdownJobConfiguration();

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                                ConfigurationObject *confObject,
                                std::vector<std::string> &errorMessages);
};

#endif // SHUTDOWNJOBCONFIGURATION_H

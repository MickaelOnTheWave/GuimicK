#ifndef SHUTDOWNJOBCONFIGURATION_H
#define SHUTDOWNJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class ShutdownJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    ShutdownJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
};

#endif // SHUTDOWNJOBCONFIGURATION_H

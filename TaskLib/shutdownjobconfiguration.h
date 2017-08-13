#ifndef SHUTDOWNJOBCONFIGURATION_H
#define SHUTDOWNJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class ShutdownJobConfiguration : public AbstractJobConfiguration
{
public:
    ShutdownJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
};

#endif // SHUTDOWNJOBCONFIGURATION_H

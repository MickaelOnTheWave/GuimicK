#ifndef SHUTDOWNJOBCONFIGURATION_H
#define SHUTDOWNJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

// TODO : add timeout property to configuration
class ShutdownJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    ShutdownJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};

#endif // SHUTDOWNJOBCONFIGURATION_H

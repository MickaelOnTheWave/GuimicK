#ifndef SHUTDOWNJOBCONFIGURATION_H
#define SHUTDOWNJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class ShutdownJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    ShutdownJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};

#endif // SHUTDOWNJOBCONFIGURATION_H

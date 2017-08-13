#ifndef WAKEJOBCONFIGURATION_H
#define WAKEJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class WakeJobConfiguration : public AbstractJobConfiguration
{
public:
    WakeJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
};

#endif // WAKEJOBCONFIGURATION_H

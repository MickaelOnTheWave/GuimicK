#ifndef WAKEJOBCONFIGURATION_H
#define WAKEJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class WakeJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    WakeJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
};

#endif // WAKEJOBCONFIGURATION_H

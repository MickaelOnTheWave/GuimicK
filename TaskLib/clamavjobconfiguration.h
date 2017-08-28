#ifndef CLAMAVJOBCONFIGURATION_H
#define CLAMAVJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class ClamAvJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    ClamAvJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
};

#endif // CLAMAVJOBCONFIGURATION_H

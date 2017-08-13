#ifndef CLAMAVJOBCONFIGURATION_H
#define CLAMAVJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class ClamAvJobConfiguration : public AbstractJobConfiguration
{
public:
    ClamAvJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
};

#endif // CLAMAVJOBCONFIGURATION_H

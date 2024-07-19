#ifndef CLAMAVJOBCONFIGURATION_H
#define CLAMAVJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class ClamAvJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    ClamAvJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
};

#endif // CLAMAVJOBCONFIGURATION_H

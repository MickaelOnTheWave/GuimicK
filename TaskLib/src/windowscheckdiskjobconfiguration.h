#ifndef WINDOWSCHECKDISKJOBCONFIGURATION_H
#define WINDOWSCHECKDISKJOBCONFIGURATION_H

#include "diskrelatedjobconfiguration.h"

class WindowsCheckDiskJobConfiguration : public DiskRelatedJobConfiguration
{
public:
    WindowsCheckDiskJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();

};

#endif // WINDOWSCHECKDISKJOBCONFIGURATION_H

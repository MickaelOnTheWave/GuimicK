#ifndef DISKSPACECHECKJOBCONFIGURATION_H
#define DISKSPACECHECKJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class DiskSpaceCheckJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::wstring DriveProperty;
    static const std::wstring TargetProperty;

    DiskSpaceCheckJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::wstring> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::wstring>& properties);
};

#endif // DISKSPACECHECKJOBCONFIGURATION_H

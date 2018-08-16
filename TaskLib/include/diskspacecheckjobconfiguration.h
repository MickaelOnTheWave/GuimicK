#ifndef DISKSPACECHECKJOBCONFIGURATION_H
#define DISKSPACECHECKJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class DiskSpaceCheckJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::string DriveProperty;
    static const std::string TargetProperty;

    DiskSpaceCheckJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // DISKSPACECHECKJOBCONFIGURATION_H

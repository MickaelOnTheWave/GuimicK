#ifndef WAKEJOBCONFIGURATION_H
#define WAKEJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class WakeJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::string TimeoutProperty;
    static const std::string MaxRetriesProperty;

    WakeJobConfiguration();

    virtual bool IsRightJob(AbstractJob* job);
    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();

    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);

    virtual void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // WAKEJOBCONFIGURATION_H

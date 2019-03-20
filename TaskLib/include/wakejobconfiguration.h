#ifndef WAKEJOBCONFIGURATION_H
#define WAKEJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class WakeJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::wstring TimeoutProperty;
    static const std::wstring MaxRetriesProperty;

    WakeJobConfiguration();

    virtual bool IsRightJob(AbstractJob* job);
    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();

    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::wstring> &errorMessages);

    virtual void FillKnownProperties(std::vector<std::wstring>& properties);
};

#endif // WAKEJOBCONFIGURATION_H

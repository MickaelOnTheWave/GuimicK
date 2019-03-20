#ifndef ABSTRACTJOBDEFAULTCONFIGURATION_H
#define ABSTRACTJOBDEFAULTCONFIGURATION_H

#include "abstractjobconfiguration.h"

class AbstractJobDefaultConfiguration : public AbstractJobConfiguration
{
public:
    static const std::wstring DebugProperty;
    static const std::wstring TitleProperty;

    AbstractJobDefaultConfiguration(const std::wstring& tag);

    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
    virtual void ConfigureJob(AbstractJob* job, ConfigurationObject* confObject,
                              std::vector<std::wstring> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::wstring>& properties);
};

#endif // ABSTRACTJOBDEFAULTCONFIGURATION_H

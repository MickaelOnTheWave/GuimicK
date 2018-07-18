#ifndef ABSTRACTJOBDEFAULTCONFIGURATION_H
#define ABSTRACTJOBDEFAULTCONFIGURATION_H

#include "abstractjobconfiguration.h"

class AbstractJobDefaultConfiguration : public AbstractJobConfiguration
{
public:
    static const std::string DebugProperty;
    static const std::string TitleProperty;

    AbstractJobDefaultConfiguration(const std::string& tag);

    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
    virtual void ConfigureJob(AbstractJob* job, ConfigurationObject* confObject,
                              std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // ABSTRACTJOBDEFAULTCONFIGURATION_H

#ifndef ABSTRACTJOBDEFAULTCONFIGURATION_H
#define ABSTRACTJOBDEFAULTCONFIGURATION_H

#include "abstractjobconfiguration.h"

class AbstractJobDefaultConfiguration : public AbstractJobConfiguration
{
public:
    static const std::string DebugProperty;

    AbstractJobDefaultConfiguration(const std::string& tag);

protected:
    virtual void ConfigureJob(AbstractJob* job, ConfigurationObject* confObject,
                              std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);

private:
    int GetDebugValue(const std::string& stringValue) const;
};

#endif // ABSTRACTJOBDEFAULTCONFIGURATION_H

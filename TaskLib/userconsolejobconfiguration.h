#ifndef USERCONSOLEJOBCONFIGURATION_H
#define USERCONSOLEJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

class UserConsoleJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    UserConsoleJobConfiguration();
    UserConsoleJobConfiguration(const std::string& tag);

protected:
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);
    void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // USERCONSOLEJOBCONFIGURATION_H

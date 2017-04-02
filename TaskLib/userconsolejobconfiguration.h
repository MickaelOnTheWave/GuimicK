#ifndef USERCONSOLEJOBCONFIGURATION_H
#define USERCONSOLEJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

class UserConsoleJobConfiguration : public AbstractJobConfiguration
{
public:
    UserConsoleJobConfiguration();
    UserConsoleJobConfiguration(const std::string& tag);

protected:
    AbstractJob* CreateConfiguredJobAfterCheck(
                        ConfigurationObject *confObject,
                        std::vector<std::string> &errorMessages);
    void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // USERCONSOLEJOBCONFIGURATION_H

#ifndef SSHCONSOLEJOBCONFIGURATION_H
#define SSHCONSOLEJOBCONFIGURATION_H

#include "userconsolejobconfiguration.h"

class SshConsoleJobConfiguration : public UserConsoleJobConfiguration
{
public:
    SshConsoleJobConfiguration();

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                            ConfigurationObject *confObject,
                            std::vector<std::string> &errorMessages);
};

#endif // SSHCONSOLEJOBCONFIGURATION_H

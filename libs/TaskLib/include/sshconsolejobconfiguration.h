#ifndef SSHCONSOLEJOBCONFIGURATION_H
#define SSHCONSOLEJOBCONFIGURATION_H

#include "userconsolejobconfiguration.h"

class SshConsoleJobConfiguration : public UserConsoleJobConfiguration
{
public:
    SshConsoleJobConfiguration();
    virtual ~SshConsoleJobConfiguration();

    virtual bool IsRightJob(AbstractJob* job);
    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                            ConfigurationObject *confObject,
                            std::vector<std::wstring> &errorMessages);
};

#endif // SSHCONSOLEJOBCONFIGURATION_H

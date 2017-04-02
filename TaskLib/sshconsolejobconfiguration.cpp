#include "sshconsolejobconfiguration.h"
#include "sshconsolejob.h"

#include "userconsolejob.h"
#include "userconsolejobconfiguration.h"

SshConsoleJobConfiguration::SshConsoleJobConfiguration()
    : UserConsoleJobConfiguration("SshConsole")
{
}

AbstractJob *SshConsoleJobConfiguration::CreateConfiguredJobAfterCheck(
                                            ConfigurationObject *confObject,
                                            std::vector<std::string> &errorMessages)
{
    AbstractJob* remoteJob = UserConsoleJobConfiguration::CreateConfiguredJobAfterCheck(
                                                confObject, errorMessages);
    return new SshConsoleJob(remoteJob->GetName(), static_cast<UserConsoleJob*>(remoteJob));
}

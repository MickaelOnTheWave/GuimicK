#include "sshconsolejobconfigurationtest.h"

#include "sshconsolejobconfiguration.h"

AbstractJobConfiguration *SshConsoleJobConfigurationTest::CreateNewConfiguration() const
{
    return new SshConsoleJobConfiguration();
}

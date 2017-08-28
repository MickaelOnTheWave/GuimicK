#ifndef SSHCONSOLEJOBCONFIGURATIONTEST_H
#define SSHCONSOLEJOBCONFIGURATIONTEST_H

#include "userconsolejobconfigurationtest.h"

class SshConsoleJobConfigurationTest : public UserConsoleJobConfigurationTest
{
    Q_OBJECT

public:
    SshConsoleJobConfigurationTest() = default;
    virtual ~SshConsoleJobConfigurationTest() = default;

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;
};

#endif // SSHCONSOLEJOBCONFIGURATIONTEST_H

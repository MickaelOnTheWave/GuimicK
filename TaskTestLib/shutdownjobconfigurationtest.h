#ifndef SHUTDOWNJOBCONFIGURATIONTEST_H
#define SHUTDOWNJOBCONFIGURATIONTEST_H

#include "abstractjobdefaultconfigurationtest.h"

class ShutdownJobConfigurationTest : public AbstractJobDefaultConfigurationTest
{
    Q_OBJECT

public:
    ShutdownJobConfigurationTest() = default;
    virtual ~ShutdownJobConfigurationTest() = default;

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;
};

#endif // SHUTDOWNJOBCONFIGURATIONTEST_H

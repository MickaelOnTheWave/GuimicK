#ifndef WAKEJOBCONFIGURATIONTEST_H
#define WAKEJOBCONFIGURATIONTEST_H

#include "abstractjobdefaultconfigurationtest.h"

class WakeJobConfigurationTest : public AbstractJobDefaultConfigurationTest
{
    Q_OBJECT

public:
    WakeJobConfigurationTest() = default;
    virtual ~WakeJobConfigurationTest() = default;

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;
};

#endif // WAKEJOBCONFIGURATIONTEST_H

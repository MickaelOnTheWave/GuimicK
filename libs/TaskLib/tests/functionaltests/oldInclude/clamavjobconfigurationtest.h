#ifndef CLAMAVJOBCONFIGURATIONTEST_H
#define CLAMAVJOBCONFIGURATIONTEST_H

#include "abstractjobdefaultconfigurationtest.h"

class ClamAvJobConfigurationTest : public AbstractJobDefaultConfigurationTest
{
    Q_OBJECT

public:
    ClamAvJobConfigurationTest() = default;
    virtual ~ClamAvJobConfigurationTest() = default;

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;
};

#endif // CLAMAVJOBCONFIGURATIONTEST_H

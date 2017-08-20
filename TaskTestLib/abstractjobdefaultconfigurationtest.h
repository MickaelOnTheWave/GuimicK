#ifndef ABSTRACTJOBDEFAULTCONFIGURATIONTEST_H
#define ABSTRACTJOBDEFAULTCONFIGURATIONTEST_H

#include "abstractjobconfigurationtest.h"

class AbstractJobDefaultConfigurationTest : public AbstractJobConfigurationTest
{
    Q_OBJECT

public:
    AbstractJobDefaultConfigurationTest() = default;
    virtual ~AbstractJobDefaultConfigurationTest() = default;

private Q_SLOTS:
    void testConfigure_ShowDebugProperty_data();
    void testConfigure_ShowDebugProperty();

private:
    void testConfigure_ShowDebugProperty(const std::string& propertyValue,
                                         const int expectedValue);

};

#endif // ABSTRACTJOBDEFAULTCONFIGURATIONTEST_H

#ifndef ABSTRACTJOBDEFAULTCONFIGURATIONTEST_H
#define ABSTRACTJOBDEFAULTCONFIGURATIONTEST_H

#include "abstractjobconfigurationtest.h"

class AbstractJobDefaultConfigurationTest : public AbstractJobConfigurationTest
{

public:
    AbstractJobDefaultConfigurationTest() = default;
    virtual ~AbstractJobDefaultConfigurationTest() = default;

private:
    void testConfigure_ShowDebugProperty_data();
    void testConfigure_ShowDebugProperty();
    void testConfigure_TitleProperty();

    void TestShowDebugProperty(const std::wstring& propertyValue,
                               const int expectedValue);
    void TestTitleProperty(const std::wstring& value);

};

#endif // ABSTRACTJOBDEFAULTCONFIGURATIONTEST_H

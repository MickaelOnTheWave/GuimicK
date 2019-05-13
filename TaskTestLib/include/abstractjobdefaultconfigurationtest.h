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
    void testConfigure_TitleProperty();

private:
    void TestShowDebugProperty(const std::wstring& propertyValue,
                               const int expectedValue);
    void TestTitleProperty(const std::wstring& value);

};

#endif // ABSTRACTJOBDEFAULTCONFIGURATIONTEST_H

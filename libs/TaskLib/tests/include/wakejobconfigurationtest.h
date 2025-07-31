#ifndef WAKEJOBCONFIGURATIONTEST_H
#define WAKEJOBCONFIGURATIONTEST_H

#include "abstractjobdefaultconfigurationtest.h"

class WakeJobConfigurationTest : public AbstractJobDefaultConfigurationTest
{
    Q_OBJECT

public:
    WakeJobConfigurationTest() = default;
    virtual ~WakeJobConfigurationTest() = default;

private Q_SLOTS:
    void testConfigure_TimeoutProperty_data();
    void testConfigure_TimeoutProperty();

    void testConfigure_RetriesProperty_data();
    void testConfigure_RetriesProperty();

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;

private:
    void TestTimeoutProperty(const std::wstring& propertyValue, const int expectedValue);
    void TestRetriesProperty(const std::wstring& propertyValue, const int expectedValue);
};

#endif // WAKEJOBCONFIGURATIONTEST_H

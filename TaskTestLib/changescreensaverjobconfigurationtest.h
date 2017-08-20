#ifndef CHANGESCREENSAVERJOBCONFIGURATIONTEST_H
#define CHANGESCREENSAVERJOBCONFIGURATIONTEST_H

#include "abstractjobdefaultconfigurationtest.h"

class ChangeScreensaverJobConfigurationTest : public AbstractJobDefaultConfigurationTest
{
    Q_OBJECT

public:
    ChangeScreensaverJobConfigurationTest() = default;
    virtual ~ChangeScreensaverJobConfigurationTest() = default;

private Q_SLOTS:

    void testConfigure_TimeoutProperty_data();
    void testConfigure_TimeoutProperty();

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;

private:

    void TestConfigure_TimeoutProperty(const std::string& propertyValue, const int expectedValue);
};

#endif // CHANGESCREENSAVERJOBCONFIGURATIONTEST_H

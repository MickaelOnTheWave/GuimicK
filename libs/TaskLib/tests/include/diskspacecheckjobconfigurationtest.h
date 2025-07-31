#ifndef DISKSPACECHECKJOBCONFIGURATIONTEST_H
#define DISKSPACECHECKJOBCONFIGURATIONTEST_H

#include "abstractjobdefaultconfigurationtest.h"

class DiskSpaceCheckJobConfigurationTest : public AbstractJobDefaultConfigurationTest
{
    Q_OBJECT

public:
    DiskSpaceCheckJobConfigurationTest() = default;
    virtual ~DiskSpaceCheckJobConfigurationTest() = default;

private Q_SLOTS:

    void testConfigure_DriveProperty();

    void testConfigure_TargetProperty_data();
    void testConfigure_TargetProperty();

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;

private:
    void TestDriveProperty(const std::wstring& propertyValue);
    void TestTargetProperty(const std::wstring& propertyValue,
                            const bool expectedValue);
};

#endif // DISKSPACECHECKJOBCONFIGURATIONTEST_H

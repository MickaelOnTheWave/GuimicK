#ifndef RSNAPSHOTBACKUPJOBCONFIGURATIONTEST_H
#define RSNAPSHOTBACKUPJOBCONFIGURATIONTEST_H

#include "abstractbackupjobconfigurationtest.h"

class RsnapshotBackupJobConfigurationTest : public AbstractBackupJobConfigurationTest
{
    Q_OBJECT

public:
    RsnapshotBackupJobConfigurationTest() = default;
    virtual ~RsnapshotBackupJobConfigurationTest() = default;

private Q_SLOTS:

    void testConfigure_RepositoryProperty();
    void testConfigure_FullConfigurationProperty();

    void testConfigure_WaitProperty_data();
    void testConfigure_WaitProperty();

    void testConfigure_TemplateConfigurationProperty();

    void testConfigure_MaxBackupCountProperty_data();
    void testConfigure_MaxBackupCountProperty();

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const;

private:
    virtual void CreateBackupItemObjectsTestData() override;

    void TestRepositoryProperty(const std::wstring& propertyValue);
    void TestFullConfigurationProperty(const std::wstring &propertyValue);
    void TestWaitProperty(const std::wstring& propertyValue, const bool expected);
    void TestTemplateConfigurationProperty(const std::wstring &propertyValue);
    void TestMaxBackupCountProperty(const std::wstring& propertyValue, const int expected);

};

#endif // RSNAPSHOTBACKUPJOBCONFIGURATIONTEST_H

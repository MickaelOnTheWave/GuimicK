#ifndef GITBACKUPJOBCONFIGURATIONTEST_H
#define GITBACKUPJOBCONFIGURATIONTEST_H

#include "abstractbackupjobconfigurationtest.h"

class GitBackupJobConfigurationTest : public AbstractBackupJobConfigurationTest
{
    Q_OBJECT

public:
    GitBackupJobConfigurationTest() = default;
    virtual ~GitBackupJobConfigurationTest() = default;

private Q_SLOTS:

    void testConfigure_WriteLogsProperty_data();
    void testConfigure_WriteLogsProperty();

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const override;

private:
    void TestWriteLogsProperty(const std::wstring& propertyValue,
                               const bool expectedValue);
};

#endif // GITBACKUPJOBCONFIGURATIONTEST_H

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

    void testConfigure_RepositoryProperty_data();
    void testConfigure_RepositoryProperty();

    void testConfigure_FullConfigurationProperty_data();
    void testConfigure_FullConfigurationProperty();

    void testConfigure_WaitProperty_data();
    void testConfigure_WaitProperty();

    void testConfigure_TemplateConfigurationProperty_data();
    void testConfigure_TemplateConfigurationProperty();

protected:
    virtual AbstractJobConfiguration* CreateNewConfiguration() const;

private:
    void testConfigure_RepositoryProperty(const std::string &propertyValue,
                                          const std::string &expectedValue);
    void testConfigure_FullConfigurationProperty(const std::string &propertyValue,
                                                 const std::string &expectedValue);
    void testConfigure_WaitProperty(const std::string& propertyValue, const bool expected);
    void testConfigure_TemplateConfigurationProperty(const std::string &propertyValue,
                                                     const std::string &expectedValue);

};

#endif // RSNAPSHOTBACKUPJOBCONFIGURATIONTEST_H

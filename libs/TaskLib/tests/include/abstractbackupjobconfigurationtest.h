#ifndef ABSTRACTBACKUPJOBCONFIGURATIONTEST_H
#define ABSTRACTBACKUPJOBCONFIGURATIONTEST_H

#include "abstractjobdefaultconfigurationtest.h"

#include "abstractbackupjob.h"
#include "configurationobject.h"

class AbstractBackupJobConfigurationTest : public AbstractJobDefaultConfigurationTest
{

public:
    AbstractBackupJobConfigurationTest() = default;
    virtual ~AbstractBackupJobConfigurationTest() = default;

private:
    void initTestCase();

    void testConfigure_TargetProperty();
    void testConfigure_JoinReports();
    void testConfigure_BackupItemObjects();

    using BackupValues = std::vector<std::pair<std::vector<std::wstring>, std::vector<std::wstring>>>;

    virtual void CreateBackupItemObjectsTestData();

    void testConfigure_TargetProperty(const std::wstring& propertyValue,
                                      const bool expectedValue);

    void testConfigure_JoinReportsProperty(const std::wstring& propertyValue,
                                           const bool expectedValue);
    void testConfigure_BackupItemObjects(
            const BackupValues& values,
            const AbstractBackupJob::BackupCollection& expectedItems,
            const std::vector<std::wstring>& expectedErrors);

    ConfigurationObject* CreateBackupConfigurationObject(const BackupValues& values);
    ConfigurationObject* CreateBackupItemObject(const std::vector<std::wstring>& properties,
                                                const std::vector<std::wstring>& values);

    void CheckCollectionsAreEqual(const AbstractBackupJob::BackupCollection& items,
                                  const AbstractBackupJob::BackupCollection& expectedItems);

    BackupValues BuildSingleBackupItem(const std::vector<std::wstring>& properties, const std::vector<std::wstring>& values);
    AbstractBackupJob::BackupCollection BuildOneItemBackupCollection(const std::wstring &source, const std::wstring &destination);

    std::wstring GetConfigurationBackupItemName() const;

    std::wstring backupItemName;
};

#endif // ABSTRACTBACKUPJOBCONFIGURATIONTEST_H

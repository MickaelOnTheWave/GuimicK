#ifndef ABSTRACTBACKUPJOBCONFIGURATIONTEST_H
#define ABSTRACTBACKUPJOBCONFIGURATIONTEST_H

#include "abstractjobdefaultconfigurationtest.h"

#include "abstractbackupjob.h"
#include "configurationobject.h"

class AbstractBackupJobConfigurationTest : public AbstractJobDefaultConfigurationTest
{
    Q_OBJECT

public:
    AbstractBackupJobConfigurationTest() = default;
    virtual ~AbstractBackupJobConfigurationTest() = default;

private Q_SLOTS:
    void initTestCase();

    void testConfigure_TargetProperty_data();
    void testConfigure_TargetProperty();

    void testConfigure_JoinReports_data();
    void testConfigure_JoinReports();

    void testConfigure_BackupItemObjects_data();
    void testConfigure_BackupItemObjects();

private:
    using BackupValues = std::vector<std::pair<std::vector<std::string>, std::vector<std::string> > >;

    void testConfigure_TargetProperty(const std::string& propertyValue,
                                      const bool expectedValue);

    void testConfigure_JoinReportsProperty(const std::string& propertyValue,
                                           const bool expectedValue);
    void testConfigure_BackupItemObjects(
            const BackupValues& values, const AbstractBackupJob::BackupCollection& expectedItems,
            const std::vector<std::string> &expectedErrors);

    ConfigurationObject* CreateBackupConfigurationObject(const BackupValues& values);
    ConfigurationObject* CreateBackupItemObject(const std::vector<std::string>& properties,
                                                const std::vector<std::string>& values);

    void CheckCollectionsAreEqual(const AbstractBackupJob::BackupCollection& items,
                                  const AbstractBackupJob::BackupCollection& expectedItems);

    BackupValues BuildSingleBackupItem(const QStringList &properties,
                                       const QStringList &values);
    AbstractBackupJob::BackupCollection BuildOneItemBackupCollection(
            const QString& source, const QString& destination);

    std::string GetConfigurationBackupItemName() const;

    std::string backupItemName;
};

#endif // ABSTRACTBACKUPJOBCONFIGURATIONTEST_H

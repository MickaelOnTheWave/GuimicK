#include "gitbackupjobconfigurationtest.h"

#include <QTest>
#include "gitbackupjob.h"
#include "gitbackupjobconfiguration.h"

using namespace std;

void GitBackupJobConfigurationTest::testConfigure_WriteLogsProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<bool>("expectedValue");

    QTest::newRow("True") << "true" << true;
    QTest::newRow("False") << "false" << false;
    QTest::newRow("Default as False") << "" << false;
    QTest::newRow("Unknown as False") << "blabla" << false;
}

void GitBackupJobConfigurationTest::testConfigure_WriteLogsProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(bool, expectedValue);
    TestWriteLogsProperty(propertyValue.toStdString(), expectedValue);
}

AbstractJobConfiguration *GitBackupJobConfigurationTest::CreateNewConfiguration() const
{
    return new GitBackupJobConfiguration();
}

void GitBackupJobConfigurationTest::TestWriteLogsProperty(const string &propertyValue,
                                                          const bool expectedValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                GitBackupJobConfiguration::WriteLogProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<GitBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->GetWriteLogsToFile() == expectedValue);

    delete job;
    delete confObject;
}

#include "diskspacecheckjobconfigurationtest.h"

#include <QTest>
#include "linuxfreespacecheckjob.h"
#include "diskspacecheckjobconfiguration.h"

using namespace std;

void DiskSpaceCheckJobConfigurationTest::testConfigure_DriveProperty()
{
    TestDriveProperty("/dev/myhd");
}

void DiskSpaceCheckJobConfigurationTest::testConfigure_TargetProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<bool>("expectedValue");

    QTest::newRow("True") << "true" << true;
    QTest::newRow("False") << "false" << false;
    QTest::newRow("Default is True") << "" << true;
    QTest::newRow("Unknown is True") << "blabla" << true;
}

void DiskSpaceCheckJobConfigurationTest::testConfigure_TargetProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(bool, expectedValue);
    TestTargetProperty(propertyValue.toStdString(), expectedValue);
}

AbstractJobConfiguration *DiskSpaceCheckJobConfigurationTest::CreateNewConfiguration() const
{
    return new DiskSpaceCheckJobConfiguration();
}

void DiskSpaceCheckJobConfigurationTest::TestDriveProperty(const string &propertyValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                DiskSpaceCheckJobConfiguration::DriveProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<LinuxFreeSpaceCheckJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->GetDrive() == propertyValue);

    delete job;
    delete confObject;
}

void DiskSpaceCheckJobConfigurationTest::TestTargetProperty(const string &propertyValue,
                                                            const bool expectedValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                DiskSpaceCheckJobConfiguration::TargetProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<LinuxFreeSpaceCheckJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->IsTargetLocal() == expectedValue);

    delete job;
    delete confObject;
}

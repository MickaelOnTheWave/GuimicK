#include "rsnapshotbackupjobconfigurationtest.h"

#include <QTest>

#include "rsnapshotbackupjobconfiguration.h"
#include "rsnapshotrawbackupjob.h"
#include "rsnapshotsmartbackupjob.h"

using namespace std;

AbstractJobConfiguration *RsnapshotBackupJobConfigurationTest::CreateNewConfiguration() const
{
    return new RsnapshotBackupJobConfiguration();
}

void RsnapshotBackupJobConfigurationTest::testConfigure_RepositoryProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<QString>("expectedValue");
    QTest::newRow("Some value") << "some/value" << "some/value";
}

void RsnapshotBackupJobConfigurationTest::testConfigure_RepositoryProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(QString, expectedValue);
    testConfigure_RepositoryProperty(propertyValue.toStdString(), expectedValue.toStdString());
}

void RsnapshotBackupJobConfigurationTest::testConfigure_FullConfigurationProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<QString>("expectedValue");
    QTest::newRow("Some value") << "some/value" << "some/value";
}

void RsnapshotBackupJobConfigurationTest::testConfigure_FullConfigurationProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(QString, expectedValue);
    testConfigure_FullConfigurationProperty(propertyValue.toStdString(), expectedValue.toStdString());
}

void RsnapshotBackupJobConfigurationTest::testConfigure_RepositoryProperty(
        const string& propertyValue, const string& expectedValue)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    confObject->SetProperty(RsnapshotBackupJobConfiguration::FullConfigurationProperty, "blabla");
    if (propertyValue != "")
        confObject->SetProperty(RsnapshotBackupJobConfiguration::RepositoryProperty, propertyValue);

    vector<string> expectedErrors;
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    auto castJob = dynamic_cast<RsnapshotRawBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetRepository(), expectedValue);

    delete job;
    delete confObject;
}

void RsnapshotBackupJobConfigurationTest::testConfigure_FullConfigurationProperty(
        const string &propertyValue,
        const string &expectedValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                RsnapshotBackupJobConfiguration::FullConfigurationProperty, propertyValue);

    vector<string> expectedErrors;
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    auto castJob = dynamic_cast<RsnapshotRawBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetConfigurationFile(), expectedValue);

    delete job;
    delete confObject;
}

void RsnapshotBackupJobConfigurationTest::testConfigure_WaitProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<bool>("expectedValue");

    QTest::newRow("True") << "true" << true;
    QTest::newRow("False") << "false" << false;
    QTest::newRow("Unknown as false") << "blabla" << false;
    QTest::newRow("Default as false") << "" << false;
}

void RsnapshotBackupJobConfigurationTest::testConfigure_WaitProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(bool, expectedValue);
    testConfigure_WaitProperty(propertyValue.toStdString(), expectedValue);
}

void RsnapshotBackupJobConfigurationTest::testConfigure_TemplateConfigurationProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<QString>("expectedValue");
    QTest::newRow("Some value") << "some/value" << "some/value";
}

void RsnapshotBackupJobConfigurationTest::testConfigure_TemplateConfigurationProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(QString, expectedValue);
    testConfigure_TemplateConfigurationProperty(propertyValue.toStdString(), expectedValue.toStdString());
}

void RsnapshotBackupJobConfigurationTest::testConfigure_WaitProperty(
        const std::string &propertyValue, const bool expected)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                RsnapshotBackupJobConfiguration::WaitProperty, propertyValue);

    vector<string> expectedErrors;
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    auto castJob = dynamic_cast<RsnapshotSmartBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetWaitBeforeRun(), expected);

    delete job;
    delete confObject;
}

void RsnapshotBackupJobConfigurationTest::testConfigure_TemplateConfigurationProperty(
        const string &propertyValue,
        const string &expectedValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                RsnapshotBackupJobConfiguration::TemplateConfigurationProperty, propertyValue);

    vector<string> expectedErrors;
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    auto castJob = dynamic_cast<RsnapshotSmartBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetTemplateConfigurationFile(), expectedValue);

    delete job;
    delete confObject;
}

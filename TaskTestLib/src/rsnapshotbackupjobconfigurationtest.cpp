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

void RsnapshotBackupJobConfigurationTest::CreateBackupItemObjectsTestData()
{
   QTest::newRow("Valid backup") << QStringList{"source", "dest"}
                                 << QStringList{"/valueSrc", "valueDst"}
                                 << "/valueSrc" << "valueDst" << QStringList();

   QTest::newRow("No source") << QStringList{"source", "dest"}
                                 << QStringList{"", "valueDst"}
                                 << "" << ""
                                 << QStringList{"Error : source is invalid"};

   QTest::newRow("No destination") << QStringList{"source", "dest"}
                                 << QStringList{"/valueSrc", ""}
                                 << "" << ""
                                 << QStringList{"Error : destination is invalid"};
   QTest::newRow("Unknown properties ignored") << QStringList{"source", "dest", "other"}
                                               << QStringList{"/valueSrc", "valueDst", "otherVal"}
                                               << "/valueSrc" << "valueDst" << QStringList();
}

void RsnapshotBackupJobConfigurationTest::testConfigure_RepositoryProperty()
{
    TestRepositoryProperty("some/value");
}

void RsnapshotBackupJobConfigurationTest::testConfigure_FullConfigurationProperty()
{
    TestFullConfigurationProperty("some/value");
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
    TestWaitProperty(propertyValue.toStdString(), expectedValue);
}

void RsnapshotBackupJobConfigurationTest::testConfigure_TemplateConfigurationProperty()
{
    TestTemplateConfigurationProperty("some/value");
}

void RsnapshotBackupJobConfigurationTest::testConfigure_MaxBackupCountProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<int>("expectedValue");

    QTest::newRow("3") << "3" << 3;
    QTest::newRow("22") << "22" << 22;
    QTest::newRow("Zero is 100 (default)") << "0" << 100;
    QTest::newRow("Negative is 1") << "-3" << 1;
    QTest::newRow("Default is 100") << "" << 100;
    QTest::newRow("Unknown is 100") << "blabla" << 100;
}

void RsnapshotBackupJobConfigurationTest::testConfigure_MaxBackupCountProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(int, expectedValue);
    TestMaxBackupCountProperty(propertyValue.toStdString(), expectedValue);
}


void RsnapshotBackupJobConfigurationTest::TestRepositoryProperty(const string& propertyValue)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    confObject->SetProperty(RsnapshotBackupJobConfiguration::FullConfigurationProperty, "blabla");
    if (propertyValue != "")
        confObject->SetProperty(RsnapshotBackupJobConfiguration::RepositoryProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<RsnapshotRawBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetRepository(), propertyValue);

    delete job;
    delete confObject;
}

void RsnapshotBackupJobConfigurationTest::TestFullConfigurationProperty(const string &propertyValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                RsnapshotBackupJobConfiguration::FullConfigurationProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<RsnapshotRawBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetConfigurationFile(), propertyValue);

    delete job;
    delete confObject;
}

void RsnapshotBackupJobConfigurationTest::TestWaitProperty(const std::string &propertyValue,
                                                           const bool expected)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                RsnapshotBackupJobConfiguration::WaitProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<RsnapshotSmartBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetWaitBeforeRun(), expected);

    delete job;
    delete confObject;
}

void RsnapshotBackupJobConfigurationTest::TestTemplateConfigurationProperty(const string &propertyValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                RsnapshotBackupJobConfiguration::TemplateConfigurationProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<RsnapshotSmartBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetTemplateConfigurationFile(), propertyValue);

    delete job;
    delete confObject;
}

void RsnapshotBackupJobConfigurationTest::TestMaxBackupCountProperty(const string &propertyValue,
                                                                     const int expected)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                RsnapshotBackupJobConfiguration::MaxBackupCountProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<RsnapshotSmartBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetMaxBackupCount(), expected);

    delete job;
    delete confObject;
}

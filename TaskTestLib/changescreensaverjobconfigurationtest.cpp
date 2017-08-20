#include "changescreensaverjobconfigurationtest.h"

#include <QTest>

#include "changescreensaverjob.h"
#include "changescreensaverjobconfiguration.h"

using namespace std;

void ChangeScreensaverJobConfigurationTest::testConfigure_TimeoutProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<int>("expectedValue");

    QTest::newRow("Valid value : 10") << "10" << 10;
    QTest::newRow("Unknown value as 0") << "blabla" << 0;
    QTest::newRow("Default value as 600") << "" << 600;
}

void ChangeScreensaverJobConfigurationTest::testConfigure_TimeoutProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(int, expectedValue);
    TestConfigure_TimeoutProperty(propertyValue.toStdString(), expectedValue);
}

AbstractJobConfiguration *ChangeScreensaverJobConfigurationTest::CreateNewConfiguration() const
{
    return new ChangeScreensaverJobConfiguration();
}

void ChangeScreensaverJobConfigurationTest::TestConfigure_TimeoutProperty(
        const std::string &propertyValue, const int expectedValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                ChangeScreensaverJobConfiguration::TimeoutProperty, propertyValue);

    vector<string> expectedErrors;
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    auto castJob = dynamic_cast<ChangeScreensaverJob*>(job);

    QVERIFY(castJob != nullptr);
    QCOMPARE(castJob->GetTimeout(), expectedValue);

    delete job;
    delete confObject;
}

#include "wakejobconfigurationtest.h"

#include <QTest>
#include "abstractwakejob.h"
#include "wakejobconfiguration.h"

using namespace std;

void WakeJobConfigurationTest::testConfigure_TimeoutProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<int>("expectedValue");

    QTest::newRow("1") << "1" << 1;
    QTest::newRow("10") << "10" << 10;
    QTest::newRow("Default is 120") << "" << 120;
    QTest::newRow("Zero is Default") << "0" << 120;
    QTest::newRow("Negative is Default") << "-1" << 120;
    QTest::newRow("Unknown is Default") << "blabla" << 120;
}

void WakeJobConfigurationTest::testConfigure_TimeoutProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(int, expectedValue);
    TestTimeoutProperty(propertyValue.toStdWString(), expectedValue);
}

void WakeJobConfigurationTest::testConfigure_RetriesProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<int>("expectedValue");

    QTest::newRow("1") << "1" << 1;
    QTest::newRow("10") << "10" << 10;
    QTest::newRow("Default is 3") << "" << 3;
    QTest::newRow("Zero is Default") << "0" << 3;
    QTest::newRow("Negative is Default") << "-1" << 3;
    QTest::newRow("Unknown is Default") << "blabla" << 3;
}

void WakeJobConfigurationTest::testConfigure_RetriesProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(int, expectedValue);
    TestRetriesProperty(propertyValue.toStdWString(), expectedValue);
}

AbstractJobConfiguration *WakeJobConfigurationTest::CreateNewConfiguration() const
{
    return new WakeJobConfiguration();
}

void WakeJobConfigurationTest::TestTimeoutProperty(const wstring &propertyValue,
                                                   const int expectedValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                WakeJobConfiguration::TimeoutProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<AbstractWakeJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->GetTimeout() == expectedValue);

    delete job;
    delete confObject;
}

void WakeJobConfigurationTest::TestRetriesProperty(const wstring& propertyValue,
                                                   const int expectedValue)
{
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                WakeJobConfiguration::MaxRetriesProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);
    auto castJob = dynamic_cast<AbstractWakeJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->GetMaxRetries() == expectedValue);

    delete job;
    delete confObject;
}

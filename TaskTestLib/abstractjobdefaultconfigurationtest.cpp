#include "abstractjobdefaultconfigurationtest.h"

#include <QTest>
#include "abstractjobdefaultconfiguration.h"
#include "debugoutput.h"

using namespace std;

void AbstractJobDefaultConfigurationTest::testConfigure_ShowDebugProperty_data()
{
    QTest::addColumn<QString>("propertyValue");
    QTest::addColumn<int>("expectedValue");

    QTest::newRow("Always") << "always" << DebugOutput::ALWAYS;
    QTest::newRow("On any error") << "onanyerror" << DebugOutput::ON_ANY_ERROR;
    QTest::newRow("On fatal error") << "onfatalerror" << DebugOutput::ON_FATAL_ERROR;
    QTest::newRow("Never") << "never" << DebugOutput::NEVER;

    QTest::newRow("True as Always") << "true" << DebugOutput::ALWAYS;
    QTest::newRow("False as Never") << "false" << DebugOutput::NEVER;

    QTest::newRow("Unknown value as Never") << "blabla" << DebugOutput::NEVER;
    QTest::newRow("Default value as Never") << "" << DebugOutput::NEVER;
}

void AbstractJobDefaultConfigurationTest::testConfigure_ShowDebugProperty()
{
    QFETCH(QString, propertyValue);
    QFETCH(int, expectedValue);

    testConfigure_ShowDebugProperty(propertyValue.toStdString(), expectedValue);
}

void AbstractJobDefaultConfigurationTest::testConfigure_ShowDebugProperty(
        const string &propertyValue, const int expectedValue)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    if (propertyValue != "")
        confObject->SetProperty(AbstractJobDefaultConfiguration::DebugProperty, propertyValue);

    AbstractJob* job = TestConfigurationWithoutErrors(confObject);

    QVERIFY(job != nullptr);
    QVERIFY(job->GetOutputDebugInformationValue() == expectedValue);

    delete job;
    delete confObject;
}

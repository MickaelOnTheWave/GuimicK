#include "remotejobsrunnerconfigurationtest.h"

#include <QTest>
#include "remotejobsrunner.h"
#include "remotejobsrunnerconfiguration.h"

RemoteJobsRunnerConfigurationTest::RemoteJobsRunnerConfigurationTest()
   : AbstractJobConfigurationTest()
{
}

RemoteJobsRunnerConfigurationTest::~RemoteJobsRunnerConfigurationTest()
{
}

void RemoteJobsRunnerConfigurationTest::testConfigure_ConfFileProperty()
{
   TestConfFileProperty("dummyConfigurationFile.txt");
}

void RemoteJobsRunnerConfigurationTest::testConfigure_TimedRunProperty_data()
{
   QTest::addColumn<QString>("propertyValue");
   QTest::addColumn<bool>("expectedValue");

   QTest::newRow("true") << "true" << true;
   QTest::newRow("false") << "false" << false;
   QTest::newRow("Default is true") << "" << true;
   QTest::newRow("Unknown is true") << "blabla" << true;
}

void RemoteJobsRunnerConfigurationTest::testConfigure_TimedRunProperty()
{
   QFETCH(QString, propertyValue);
   QFETCH(bool, expectedValue);
   TestTimedRunProperty(propertyValue.toStdString(), expectedValue);
}

AbstractJobConfiguration*RemoteJobsRunnerConfigurationTest::CreateNewConfiguration() const
{
   return new RemoteJobsRunnerConfiguration();
}

void RemoteJobsRunnerConfigurationTest::TestConfFileProperty(const std::string& propertyValue)
{
   ConfigurationObject* confObject = CreateSimpleConfigurationObject(
               RemoteJobsRunnerConfiguration::ConfFileProperty, propertyValue);

   AbstractJob* job = TestConfigurationWithoutErrors(confObject);
   auto castJob = dynamic_cast<RemoteJobsRunner*>(job);

   QVERIFY(castJob != nullptr);
   QCOMPARE(castJob->GetConfigurationFile(), propertyValue);

   delete job;
   delete confObject;
}

void RemoteJobsRunnerConfigurationTest::TestTimedRunProperty(const std::string& propertyValue,
                                                             const bool expectedValue)
{
   ConfigurationObject* confObject = CreateSimpleConfigurationObject(
               RemoteJobsRunnerConfiguration::TimedJobsProperty, propertyValue);

   AbstractJob* job = TestConfigurationWithoutErrors(confObject);
   auto castJob = dynamic_cast<RemoteJobsRunner*>(job);

   QVERIFY(castJob != nullptr);
   QCOMPARE(castJob->GetIsWorkListTimed(), expectedValue);

   delete job;
   delete confObject;
}

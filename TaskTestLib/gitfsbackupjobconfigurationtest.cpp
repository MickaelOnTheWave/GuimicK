#include "gitfsbackupjobconfigurationtest.h"

#include <QTest>

#include "gitfsbackupjob.h"
#include "gitfsbackupjobconfiguration.h"

using namespace std;

void GitFsBackupJobConfigurationTest::testConfigure_JoinReports_True()
{
    testConfigure_JoinReportsProperty("true", true);
}

void GitFsBackupJobConfigurationTest::testConfigure_JoinReports_False()
{
    testConfigure_JoinReportsProperty("false", false);
}

void GitFsBackupJobConfigurationTest::testConfigure_JoinReports_TrueAsDefault()
{
    testConfigure_JoinReportsProperty("", true);
}

void GitFsBackupJobConfigurationTest::testConfigure_JoinReports_TrueOnUnknown()
{
    testConfigure_JoinReportsProperty("blabla", true);
}

AbstractJobConfiguration *GitFsBackupJobConfigurationTest::CreateNewConfiguration() const
{
    return new GitFsBackupJobConfiguration();
}

void GitFsBackupJobConfigurationTest::testConfigure_JoinReportsProperty(const string &propertyValue,
                                                                        const bool expectedValue)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    if (propertyValue != "")
        confObject->SetProperty(AbstractBackupJobConfiguration::JoinReportsProperty, propertyValue);

    vector<string> expectedErrors;
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    auto castJob = dynamic_cast<GitFsBackupJob*>(job);

    QVERIFY(castJob != nullptr);
    QVERIFY(castJob->IsReportJoint() == expectedValue);

    delete job;
    delete confObject;
}

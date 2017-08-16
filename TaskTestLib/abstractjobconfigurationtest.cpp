#include "abstractjobconfigurationtest.h"

#include <QTest>

#include "abstractjob.h"

using namespace std;

AbstractJobConfigurationTest::AbstractJobConfigurationTest(const string &dataPrefix,
                                                           const string &errorPrefix)
    : QtTestSuite(dataPrefix, errorPrefix)
{
}

AbstractJobConfigurationTest::~AbstractJobConfigurationTest()
{
}

void AbstractJobConfigurationTest::init()
{

}

void AbstractJobConfigurationTest::cleanup()
{

}

void AbstractJobConfigurationTest::testConfigure_NullConfiguration()
{
    vector<string> expectedErrors = {"Empty configuration"};
    AbstractJob* job = TestConfiguration(nullptr, expectedErrors);
    QVERIFY(job == nullptr);
    delete job;
}

AbstractJob* AbstractJobConfigurationTest::TestConfiguration(ConfigurationObject *confObject,
                                                             vector<string> &expectedErrorMessages)
{
    vector<string> errorMessages;
    AbstractJob* job = RunConfiguration(confObject, errorMessages);
    CheckErrorMessages(errorMessages, expectedErrorMessages);
    return job;
}

AbstractJob* AbstractJobConfigurationTest::RunConfiguration(ConfigurationObject *confObject,
                                                            vector<string> &errorMessages)
{
    AbstractJobConfiguration* configuration = CreateNewConfiguration();
    AbstractJob* job = configuration->CreateConfiguredJob(confObject, errorMessages);

    delete configuration;
    return job;
}

void AbstractJobConfigurationTest::CheckErrorMessages(const std::vector<string> &errorMessages,
                                                      const std::vector<string> &expectedErrorMessages)
{
    QCOMPARE(errorMessages.size(), expectedErrorMessages.size());

    bool areErrorsEqual = std::equal(errorMessages.begin(), errorMessages.end(),                                     expectedErrorMessages.begin());
    QVERIFY2(areErrorsEqual == true, "Error messages are different");
}

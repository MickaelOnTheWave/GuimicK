#include "abstractjobconfigurationtest.h"

#include <QTest>

#include "abstractjob.h"

using namespace std;

static const string nullConfigurationError = "Invalid configuration";
static const string unknownPropertyError = "Unknown property";
static const string unknownObjectError = "Unknown object";

AbstractJobConfigurationTest::AbstractJobConfigurationTest()
    : QtTestSuite("", "")
{
}

AbstractJobConfigurationTest::~AbstractJobConfigurationTest()
{
}

void AbstractJobConfigurationTest::testConfigure_NullConfiguration()
{
    vector<string> expectedErrors = {nullConfigurationError};
    AbstractJob* job = TestConfiguration(nullptr, expectedErrors);
    QVERIFY(job == nullptr);
    delete job;
}

void AbstractJobConfigurationTest::testConfigure_UnknownProperty()
{
    const string propertyName = "unknownProperty";
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                propertyName, "value");

    vector<string> expectedErrors = {BuildUnknownError("property", propertyName)};
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    QVERIFY(job != nullptr);

    delete job;
    delete confObject;
}

void AbstractJobConfigurationTest::testConfigure_UnknownSubObject()
{
    const string objectName = "unknownObject";
    ConfigurationObject* confObject = new ConfigurationObject();
    confObject->AddObject(new ConfigurationObject(objectName));

    vector<string> expectedErrors = {BuildUnknownError("sub object", objectName)};
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    QVERIFY(job != nullptr);

    delete job;
    delete confObject;

}

AbstractJob* AbstractJobConfigurationTest::TestConfiguration(
        ConfigurationObject *confObject, const vector<string>& expectedErrorMessages)
{
    vector<string> errorMessages;
    AbstractJob* job = RunConfiguration(confObject, errorMessages);
    CheckErrorMessages(errorMessages, expectedErrorMessages);
    return job;
}

AbstractJob *AbstractJobConfigurationTest::TestConfigurationWithoutErrors(
        ConfigurationObject *confObject)
{
    vector<string> expectedErrors;
    return TestConfiguration(confObject, expectedErrors);
}

ConfigurationObject *AbstractJobConfigurationTest::CreateSimpleConfigurationObject(
        const string &property, const string &value)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    if (property != "")
        confObject->SetProperty(property, value);
    return confObject;
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

    bool areErrorsEqual = std::equal(errorMessages.begin(), errorMessages.end(),
                                     expectedErrorMessages.begin());
    QVERIFY2(areErrorsEqual == true, "Error messages are different");
}

string AbstractJobConfigurationTest::BuildUnknownError(const string &object, const string &name) const
{
    AbstractJobConfiguration* dummyInstance = CreateNewConfiguration();
    string message = "Warning : ";
    message += object + " \"" + name + "\" is not handled by job \"";
    message += dummyInstance->GetTagName() + "\"";

    delete dummyInstance;
    return message;
}

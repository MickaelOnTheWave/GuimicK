#include "abstractjobconfigurationtest.h"

#include <catch2/catch.hpp>

#include "abstractjob.h"

using namespace std;

static const wstring nullConfigurationError = L"Invalid configuration";
static const wstring unknownPropertyError = L"Unknown property";
static const wstring unknownObjectError = L"Unknown object";

AbstractJobConfigurationTest::AbstractJobConfigurationTest()
{
}

void AbstractJobConfigurationTest::testConfigure_NullConfiguration()
{
    vector<wstring> expectedErrors = {nullConfigurationError};
    AbstractJob* job = TestConfiguration(nullptr, expectedErrors);
    REQUIRE(job == nullptr);
    delete job;
}

void AbstractJobConfigurationTest::testConfigure_UnknownProperty()
{
    const wstring propertyName = L"unknownProperty";
    ConfigurationObject* confObject = CreateSimpleConfigurationObject(
                propertyName, L"value");

    vector<wstring> expectedErrors = {BuildUnknownError(L"property", propertyName)};
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    REQUIRE(job != nullptr);

    delete job;
    delete confObject;
}

void AbstractJobConfigurationTest::testConfigure_UnknownSubObject()
{
    const wstring objectName = L"unknownObject";
    ConfigurationObject* confObject = new ConfigurationObject();
    confObject->AddObject(new ConfigurationObject(objectName));

    vector<wstring> expectedErrors = {BuildUnknownError(L"sub object", objectName)};
    AbstractJob* job = TestConfiguration(confObject, expectedErrors);
    REQUIRE(job != nullptr);

    delete job;
    delete confObject;

}

AbstractJob* AbstractJobConfigurationTest::TestConfiguration(
        ConfigurationObject *confObject, const vector<wstring>& expectedErrorMessages)
{
    vector<wstring> errorMessages;
    AbstractJob* job = RunConfiguration(confObject, errorMessages);
    CheckErrorMessages(errorMessages, expectedErrorMessages);
    return job;
}

AbstractJob *AbstractJobConfigurationTest::TestConfigurationWithoutErrors(
        ConfigurationObject *confObject)
{
    vector<wstring> expectedErrors;
    return TestConfiguration(confObject, expectedErrors);
}

ConfigurationObject *AbstractJobConfigurationTest::CreateSimpleConfigurationObject(
      const wstring& property, const wstring& value)
{
    ConfigurationObject* confObject = new ConfigurationObject();
    if (property != L"")
        confObject->SetProperty(property, value);
    return confObject;
}

AbstractJob* AbstractJobConfigurationTest::RunConfiguration(
      ConfigurationObject *confObject,
      vector<wstring> &errorMessages)
{
    AbstractJobConfiguration* configuration = CreateNewConfiguration();
    AbstractJob* job = configuration->CreateConfiguredJob(confObject, errorMessages);

    delete configuration;
    return job;
}

void AbstractJobConfigurationTest::CheckErrorMessages(
      const vector<wstring> &errorMessages,
      const vector<wstring> &expectedErrorMessages)
{
    REQUIRE(errorMessages.size() == expectedErrorMessages.size());

    const bool areErrorsEqual = std::equal(errorMessages.begin(), errorMessages.end(),
                                           expectedErrorMessages.begin());
    REQUIRE(areErrorsEqual == true);
}

wstring AbstractJobConfigurationTest::BuildUnknownError(const wstring& object,
                                                        const wstring& name) const
{
    AbstractJobConfiguration* dummyInstance = CreateNewConfiguration();
    wstring message = L"Warning : ";
    message += object + L" \"" + name + L"\" is not handled by job \"";
    message += dummyInstance->GetTagName() + L"\"";

    delete dummyInstance;
    return message;
}

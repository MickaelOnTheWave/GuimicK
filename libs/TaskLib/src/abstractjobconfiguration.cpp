#include "abstractjobconfiguration.h"

#include <algorithm>
#include <sstream>

using namespace std;

static const wstring nullConfigurationError = L"Invalid configuration";

AbstractJobConfiguration::AbstractJobConfiguration(const std::wstring &tag)
    : jobTag(tag)
{
}

AbstractJobConfiguration::~AbstractJobConfiguration()
{
}

std::wstring AbstractJobConfiguration::GetTagName() const
{
    return jobTag;
}

AbstractJob *AbstractJobConfiguration::CreateConfiguredJob(
                            ConfigurationObject *confObject,
                            std::vector<std::wstring> &errorMessages)
{
    if (confObject == NULL)
    {
        errorMessages.push_back(nullConfigurationError);
        return NULL;
    }

    CheckKnownProperties(confObject, errorMessages);
    CheckKnownSubObjects(confObject, errorMessages);
    return CreateConfiguredJobAfterCheck(confObject, errorMessages);
}

AbstractJob *AbstractJobConfiguration::CreateConfiguredJobAfterCheck(
        ConfigurationObject *confObject,
        std::vector<wstring> &errorMessages)
{
    AnalyzeConfiguration(confObject);
    AbstractJob* job = CreateJob();
    ConfigureJob(job, confObject, errorMessages);
    return job;
}

void AbstractJobConfiguration::AnalyzeConfiguration(ConfigurationObject *)
{
}

void AbstractJobConfiguration::ConfigureJob(AbstractJob *,
                                            ConfigurationObject *, vector<wstring> &)
{
}

void AbstractJobConfiguration::FillKnownProperties(vector<wstring> &)
{
}

void AbstractJobConfiguration::FillKnownSubObjects(vector<wstring>&)
{
}

void AbstractJobConfiguration::CheckKnownProperties(ConfigurationObject *confObject,
                                                    vector<wstring> &errorMessages)
{
    vector<wstring> knownProperties;
    FillKnownProperties(knownProperties);
    FillNumberedProperties(knownProperties);

    map<wstring, wstring>::iterator itProperty = confObject->BeginProperties();
    for (; itProperty!=confObject->EndProperties(); ++itProperty)
    {
        if (HasValue(knownProperties, itProperty->first) == false)
            errorMessages.push_back(BuildErrorMessage(L"property", itProperty->first));
    }
}

void AbstractJobConfiguration::CheckKnownSubObjects(ConfigurationObject *confObject,
                                                    std::vector<std::wstring> &errorMessages)
{
    vector<wstring> knownObjects;
    FillKnownSubObjects(knownObjects);

    list<ConfigurationObject*>::iterator it = confObject->BeginObjects();
    for (; it!=confObject->EndObjects(); ++it)
    {
        if (HasValue(knownObjects, (*it)->GetName()) == false)
            errorMessages.push_back(BuildErrorMessage(L"sub object", (*it)->GetName()));
    }
}

void AbstractJobConfiguration::FillNumberedProperties(std::vector<wstring> &objects)
{
    const size_t supportedPropertiesCount = objects.size();

    for (size_t i=0; i<supportedPropertiesCount; ++i)
    {
        wstringstream numberedProperty;
        numberedProperty << "param" << i;
        objects.push_back(numberedProperty.str());
    }
}

bool AbstractJobConfiguration::HasValue(const std::vector<wstring> &collection,
                                        const wstring &value) const
{
    vector<wstring>::const_iterator it = find(collection.begin(), collection.end(), value);
    return (it != collection.end());
}

wstring AbstractJobConfiguration::BuildErrorMessage(const wstring &objectType,
                                                   const wstring &objectName)
{
    wstring message = L"Warning : ";
    message += objectType + L" \"" + objectName + L"\" is not handled by job \"";
    message += jobTag + L"\"";
    return message;
}

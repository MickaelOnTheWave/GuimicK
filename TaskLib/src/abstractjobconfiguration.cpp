#include "abstractjobconfiguration.h"

#include <algorithm>
#include <sstream>

using namespace std;

static const string nullConfigurationError = "Invalid configuration";

AbstractJobConfiguration::AbstractJobConfiguration(const std::string &tag)
    : jobTag(tag)
{
}

AbstractJobConfiguration::~AbstractJobConfiguration()
{
}

std::string AbstractJobConfiguration::GetTagName() const
{
    return jobTag;
}

AbstractJob *AbstractJobConfiguration::CreateConfiguredJob(
                            ConfigurationObject *confObject,
                            std::vector<std::string> &errorMessages)
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
        std::vector<string> &errorMessages)
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
                                            ConfigurationObject *, vector<string> &)
{
}

void AbstractJobConfiguration::FillKnownProperties(vector<string> &)
{
}

void AbstractJobConfiguration::FillKnownSubObjects(vector<string>&)
{
}

void AbstractJobConfiguration::CheckKnownProperties(ConfigurationObject *confObject,
                                                    vector<string> &errorMessages)
{
    vector<string> knownProperties;
    FillKnownProperties(knownProperties);
    FillNumberedProperties(knownProperties);

    map<string, string>::iterator itProperty = confObject->propertyList.begin();
    for (; itProperty!=confObject->propertyList.end(); ++itProperty)
    {
        if (HasValue(knownProperties, itProperty->first) == false)
            errorMessages.push_back(BuildErrorMessage("property", itProperty->first));
    }
}

void AbstractJobConfiguration::CheckKnownSubObjects(ConfigurationObject *confObject,
                                                    std::vector<std::string> &errorMessages)
{
    vector<string> knownObjects;
    FillKnownSubObjects(knownObjects);

    list<ConfigurationObject*>::iterator it = confObject->objectList.begin();
    for (; it!=confObject->objectList.end(); ++it)
    {
        if (HasValue(knownObjects, (*it)->name) == false)
            errorMessages.push_back(BuildErrorMessage("sub object", (*it)->name));
    }
}

void AbstractJobConfiguration::FillNumberedProperties(std::vector<string> &objects)
{
    unsigned int supportedPropertiesCount = objects.size();

    for (unsigned int i=0; i<supportedPropertiesCount; ++i)
    {
        stringstream numberedProperty;
        numberedProperty << "param" << i;
        objects.push_back(numberedProperty.str());
    }
}

bool AbstractJobConfiguration::HasValue(const std::vector<string> &collection,
                                        const string &value) const
{
    vector<string>::const_iterator it = find(collection.begin(), collection.end(), value);
    return (it != collection.end());
}

string AbstractJobConfiguration::BuildErrorMessage(const string &objectType,
                                                   const string &objectName)
{
    string message = "Warning : ";
    message += objectType + " \"" + objectName + "\" is not handled by job \"";
    message += jobTag + "\"";
    return message;
}

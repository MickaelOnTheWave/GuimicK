#include "abstractjobdefaultconfiguration.h"

using namespace std;

const string AbstractJobDefaultConfiguration::DebugProperty = "showDebugInformation";

AbstractJobDefaultConfiguration::AbstractJobDefaultConfiguration(const std::string &tag)
    : AbstractJobConfiguration(tag)
{
}

void AbstractJobDefaultConfiguration::ConfigureJob(AbstractJob *job,
                                                   ConfigurationObject *confObject,
                                                   std::vector<std::string> &)
{
    const string showDebugInformation(confObject->GetProperty(DebugProperty));
    if (showDebugInformation != "")
    {
        const int debugValue = GetDebugValue(showDebugInformation);
        job->SetOutputDebugInformation(debugValue);
    }
}

void AbstractJobDefaultConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    properties.push_back(DebugProperty);
}

int AbstractJobDefaultConfiguration::GetDebugValue(const string &stringValue) const
{
    if (stringValue == "true")
        return DebugOutput::ALWAYS;
    else if (stringValue == "false")
        return DebugOutput::NEVER;
    else if (stringValue == "always")
        return DebugOutput::ALWAYS;
    else if (stringValue == "onanyerror")
        return DebugOutput::ON_ANY_ERROR;
    else if (stringValue == "onfatalerror")
        return DebugOutput::ON_FATAL_ERROR;
    else if (stringValue == "never")
        return DebugOutput::NEVER;
    else
        return DebugOutput::NEVER;
}

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
        const int debugValue = DebugOutput::GetValue(showDebugInformation);
        job->SetOutputDebugInformation(debugValue);
    }
}

void AbstractJobDefaultConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    properties.push_back(DebugProperty);
}

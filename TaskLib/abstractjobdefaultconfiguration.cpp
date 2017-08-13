#include "abstractjobdefaultconfiguration.h"

using namespace std;

static const string debugProperty = "showDebugInformation";

AbstractJobDefaultConfiguration::AbstractJobDefaultConfiguration(const std::string &tag)
    : AbstractJobConfiguration(tag)
{
}

void AbstractJobDefaultConfiguration::ConfigureJob(AbstractJob *job,
                                                   ConfigurationObject *confObject,
                                                   std::vector<std::string> &)
{
    const string showDebugInformation(confObject->GetProperty(debugProperty));

    // TODO change this
    const int debugValue = (showDebugInformation == "true") ? DebugOutput::ALWAYS : DebugOutput::NEVER;
    job->SetOutputDebugInformation(debugValue);
}

void AbstractJobDefaultConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    properties.push_back(debugProperty);
}

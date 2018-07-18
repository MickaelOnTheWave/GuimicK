#include "abstractjobdefaultconfiguration.h"

using namespace std;

const string AbstractJobDefaultConfiguration::DebugProperty = "showDebugInformation";
const string AbstractJobDefaultConfiguration::TitleProperty = "title";

AbstractJobDefaultConfiguration::AbstractJobDefaultConfiguration(const std::string &tag)
    : AbstractJobConfiguration(tag)
{
}

ConfigurationObject* AbstractJobDefaultConfiguration::CreateConfigurationObject(AbstractJob* job)
{
   ConfigurationObject* confObject = new ConfigurationObject(jobTag);
   confObject->SetProperty(TitleProperty, job->GetName());
   return confObject;
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

    const string title(confObject->GetProperty(TitleProperty));
    if (title != "")
        job->SetName(title);
}

void AbstractJobDefaultConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    properties.push_back(DebugProperty);
    properties.push_back(TitleProperty);
}

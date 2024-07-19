#include "abstractjobdefaultconfiguration.h"

using namespace std;

const wstring AbstractJobDefaultConfiguration::DebugProperty = L"showDebugInformation";
const wstring AbstractJobDefaultConfiguration::TitleProperty = L"title";

AbstractJobDefaultConfiguration::AbstractJobDefaultConfiguration(const std::wstring &tag)
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
                                                   std::vector<std::wstring> &)
{
    const wstring showDebugInformation(confObject->GetProperty(DebugProperty));
    if (showDebugInformation != L"")
    {
        const int debugValue = DebugOutput::GetValue(showDebugInformation);
        job->SetOutputDebugInformation(debugValue);
    }

    const wstring title(confObject->GetProperty(TitleProperty));
    if (title != L"")
        job->SetName(title);
}

void AbstractJobDefaultConfiguration::FillKnownProperties(std::vector<std::wstring> &properties)
{
    properties.push_back(DebugProperty);
    properties.push_back(TitleProperty);
}

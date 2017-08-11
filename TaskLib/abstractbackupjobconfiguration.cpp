#include "abstractbackupjobconfiguration.h"

using namespace std;

AbstractBackupJobConfiguration::AbstractBackupJobConfiguration(const std::string &tag)
    : AbstractJobConfiguration(tag)
{
}

void AbstractBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    properties.push_back("showDebugInformation");
}

void AbstractBackupJobConfiguration::ConfigureJob(AbstractBackupJob *job,
                                                  ConfigurationObject *confObject)
{
    const string showDebugInformation(confObject->GetProperty("showDebugInformation"));

    // TODO change this
    const int debugValue = (showDebugInformation == "true") ? DebugOutput::ALWAYS : DebugOutput::NEVER;
    job->SetOutputDebugInformation(debugValue);
}

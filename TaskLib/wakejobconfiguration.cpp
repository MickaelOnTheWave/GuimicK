#include "wakejobconfiguration.h"

#include <stdlib.h>
#include "wakejob.h"

using namespace std;

const std::string WakeJobConfiguration::TimeoutProperty = "timeout";
const std::string WakeJobConfiguration::MaxRetriesProperty = "maxRetries";

WakeJobConfiguration::WakeJobConfiguration()
    : AbstractJobDefaultConfiguration("Wake")
{
}

AbstractJob *WakeJobConfiguration::CreateJob()
{
    return new WakeJob();
}

void WakeJobConfiguration::ConfigureJob(AbstractJob *job,
                                                  ConfigurationObject *confObject,
                                                  std::vector<string> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    const string timeout = confObject->GetFirstProperty(TimeoutProperty, "param0");
    const string maxRetries = confObject->GetProperty(MaxRetriesProperty);

    WakeJob* castJob = static_cast<WakeJob*>(job);

    int timeoutValue = atoi(timeout.c_str());
    if (timeoutValue > 0)
        castJob->SetTimeout(timeoutValue);

    int retriesValue = atoi(maxRetries.c_str());
    if (retriesValue > 0)
        castJob->SetMaxRetries(retriesValue);
}

void WakeJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(TimeoutProperty);
    properties.push_back(MaxRetriesProperty);
}

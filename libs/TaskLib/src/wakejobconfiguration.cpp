#include "wakejobconfiguration.h"

#include <stdlib.h>
#include "consolewakejob.h"
#include "libwakejob.h"
#include "stringtools.h"

using namespace std;

const wstring WakeJobConfiguration::TimeoutProperty = L"timeout";
const wstring WakeJobConfiguration::MaxRetriesProperty = L"maxRetries";

WakeJobConfiguration::WakeJobConfiguration()
    : AbstractJobDefaultConfiguration(L"Wake")
{
}

bool WakeJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<AbstractWakeJob*>(job) != NULL);
}

ConfigurationObject* WakeJobConfiguration::CreateConfigurationObject(AbstractJob* job)
{
   ConfigurationObject* confObject = AbstractJobDefaultConfiguration::CreateConfigurationObject(job);

   AbstractWakeJob* wakeJob = dynamic_cast<AbstractWakeJob*>(job);
   confObject->SetProperty(TimeoutProperty, wakeJob->GetTimeout());
   confObject->SetProperty(MaxRetriesProperty, wakeJob->GetMaxRetries());
   return confObject;
}

AbstractJob *WakeJobConfiguration::CreateJob()
{
    return new LibWakeJob();
}

void WakeJobConfiguration::ConfigureJob(AbstractJob *job,
                                        ConfigurationObject *confObject,
                                        std::vector<wstring> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    const wstring timeout = confObject->GetFirstProperty(TimeoutProperty, L"param0");
    const wstring maxRetries = confObject->GetProperty(MaxRetriesProperty);

    AbstractWakeJob* castJob = static_cast<AbstractWakeJob*>(job);

    int timeoutValue = StringTools::ToInt(timeout);
    if (timeoutValue > 0)
        castJob->SetTimeout(timeoutValue);

    int retriesValue = StringTools::ToInt(maxRetries);
    if (retriesValue > 0)
        castJob->SetMaxRetries(retriesValue);
}

void WakeJobConfiguration::FillKnownProperties(std::vector<wstring> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(TimeoutProperty);
    properties.push_back(MaxRetriesProperty);
}

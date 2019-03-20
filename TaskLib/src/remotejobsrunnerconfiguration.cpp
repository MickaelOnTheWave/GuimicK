#include "remotejobsrunnerconfiguration.h"

#include "remotejobsrunner.h"

using namespace std;

const wstring RemoteJobsRunnerConfiguration::ConfFileProperty = L"conffile";
const wstring RemoteJobsRunnerConfiguration::TimedJobsProperty = L"timedJobs";

static const wstring defaultConfigurationFile = L".taskmanager";

RemoteJobsRunnerConfiguration::RemoteJobsRunnerConfiguration()
   : AbstractJobConfiguration(L"RemoteRunner")
{
}

bool RemoteJobsRunnerConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<RemoteJobsRunner*>(job) != NULL);
}

ConfigurationObject* RemoteJobsRunnerConfiguration::CreateConfigurationObject(AbstractJob* job)
{
   RemoteJobsRunner* castJob = static_cast<RemoteJobsRunner*>(job);
   ConfigurationObject* confObject = new ConfigurationObject(jobTag);

   confObject->SetProperty(ConfFileProperty, castJob->GetConfigurationFile());
   confObject->SetProperty(TimedJobsProperty, castJob->GetIsWorkListTimed() ? L"true" : L"false");
   return confObject;
}

AbstractJob* RemoteJobsRunnerConfiguration::CreateJob()
{
   return new RemoteJobsRunner();
}

void RemoteJobsRunnerConfiguration::ConfigureJob(AbstractJob* job, ConfigurationObject* confObject,
                                                 vector<wstring>& errorMessages)
{
   AbstractJobConfiguration::ConfigureJob(job, confObject, errorMessages);
   wstring conffile = confObject->GetFirstProperty(ConfFileProperty, L"param0");
   if (conffile == L"")
      conffile = defaultConfigurationFile;

   const wstring timedRun = confObject->GetProperty(TimedJobsProperty);

   RemoteJobsRunner* castJob = static_cast<RemoteJobsRunner*>(job);
   castJob->SetConfigurationFile(conffile);
   castJob->SetIsWorkListTimed(timedRun != L"false");
}

void RemoteJobsRunnerConfiguration::FillKnownProperties(std::vector<std::wstring>& properties)
{
   AbstractJobConfiguration::FillKnownProperties(properties);
   properties.push_back(ConfFileProperty);
   properties.push_back(TimedJobsProperty);
}

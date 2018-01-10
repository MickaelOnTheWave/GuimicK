#include "remotejobsrunnerconfiguration.h"

#include "remotejobsrunner.h"

using namespace std;

const string RemoteJobsRunnerConfiguration::ConfFileProperty = "conffile";
const string RemoteJobsRunnerConfiguration::TimedJobsProperty = "timedJobs";

static const string defaultConfigurationFile = ".taskmanager";

RemoteJobsRunnerConfiguration::RemoteJobsRunnerConfiguration()
   : AbstractJobConfiguration("RemoteRunner")
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
   confObject->SetProperty(TimedJobsProperty, castJob->GetIsWorkListTimed() ? "true" : "false");
   return confObject;
}

AbstractJob* RemoteJobsRunnerConfiguration::CreateJob()
{
   return new RemoteJobsRunner();
}

void RemoteJobsRunnerConfiguration::ConfigureJob(AbstractJob* job, ConfigurationObject* confObject,
                                                 vector<string>& errorMessages)
{
   AbstractJobConfiguration::ConfigureJob(job, confObject, errorMessages);
   string conffile = confObject->GetFirstProperty(ConfFileProperty, "param0");
   if (conffile == "")
      conffile = defaultConfigurationFile;

   const string timedRun = confObject->GetProperty(TimedJobsProperty);

   RemoteJobsRunner* castJob = static_cast<RemoteJobsRunner*>(job);
   castJob->SetConfigurationFile(conffile);
   castJob->SetIsWorkListTimed(timedRun != "false");
}

void RemoteJobsRunnerConfiguration::FillKnownProperties(std::vector<std::string>& properties)
{
   AbstractJobConfiguration::FillKnownProperties(properties);
   properties.push_back(ConfFileProperty);
   properties.push_back(TimedJobsProperty);
}

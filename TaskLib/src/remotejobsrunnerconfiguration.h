#ifndef REMOTEJOBSRUNNERCONFIGURATION_H
#define REMOTEJOBSRUNNERCONFIGURATION_H

#include "abstractjobconfiguration.h"

class RemoteJobsRunnerConfiguration : public AbstractJobConfiguration
{
public:
   static const std::string ConfFileProperty;
   static const std::string TimedJobsProperty;

   RemoteJobsRunnerConfiguration();

   virtual bool IsRightJob(AbstractJob *job);
   virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
   virtual AbstractJob* CreateJob();
   virtual void ConfigureJob(AbstractJob* job,
                             ConfigurationObject *confObject,
                             std::vector<std::string> &errorMessages);
   virtual void FillKnownProperties(std::vector<std::string>& properties);
};

#endif // REMOTEJOBSRUNNERCONFIGURATION_H

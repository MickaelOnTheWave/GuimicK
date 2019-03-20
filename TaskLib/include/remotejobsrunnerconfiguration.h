#ifndef REMOTEJOBSRUNNERCONFIGURATION_H
#define REMOTEJOBSRUNNERCONFIGURATION_H

#include "abstractjobconfiguration.h"

class RemoteJobsRunnerConfiguration : public AbstractJobConfiguration
{
public:
   static const std::wstring ConfFileProperty;
   static const std::wstring TimedJobsProperty;

   RemoteJobsRunnerConfiguration();

   virtual bool IsRightJob(AbstractJob *job);
   virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

protected:
   virtual AbstractJob* CreateJob();
   virtual void ConfigureJob(AbstractJob* job,
                             ConfigurationObject *confObject,
                             std::vector<std::wstring> &errorMessages);
   virtual void FillKnownProperties(std::vector<std::wstring>& properties);
};

#endif // REMOTEJOBSRUNNERCONFIGURATION_H

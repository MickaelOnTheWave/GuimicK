#ifndef JOBFACTORY_H
#define JOBFACTORY_H

#include <vector>

#include "abstractjobconfiguration.h"

class JobFactory
{
public:
   JobFactory();
   ~JobFactory();

   AbstractJob *CreateJob(ConfigurationObject* object,
                          std::vector<std::wstring>& errorMessages);

   ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

private:
   void FillSupportedJobsList();
   void FreeSupportedJobsList();
   AbstractJobConfiguration *GetConfiguration(const std::wstring &jobTab);
   AbstractJobConfiguration *GetConfiguration(AbstractJob* job);

   std::vector<AbstractJobConfiguration*> supportedJobs;
};

#endif // JOBFACTORY_H

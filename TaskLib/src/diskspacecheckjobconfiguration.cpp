#include "diskspacecheckjobconfiguration.h"
#include "linuxfreespacecheckjob.h"

using namespace std;

const wstring DiskSpaceCheckJobConfiguration::TargetProperty = L"localTarget";

DiskSpaceCheckJobConfiguration::DiskSpaceCheckJobConfiguration()
    : DiskRelatedJobConfiguration(L"DiskSpaceCheck")
{
}

bool DiskSpaceCheckJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<LinuxFreeSpaceCheckJob*>(job) != NULL);
}

ConfigurationObject* DiskSpaceCheckJobConfiguration::CreateConfigurationObject(
      AbstractJob* job)
{
   ConfigurationObject* confObject = DiskRelatedJobConfiguration::CreateConfigurationObject(job);
   LinuxFreeSpaceCheckJob* castJob = static_cast<LinuxFreeSpaceCheckJob*>(job);

   const wstring targetValue = (castJob->IsTargetLocal() ? L"true" : L"false");
   confObject->SetProperty(TargetProperty, targetValue);

   return confObject;
}

AbstractJob *DiskSpaceCheckJobConfiguration::CreateJob()
{
    return new LinuxFreeSpaceCheckJob();
}

void DiskSpaceCheckJobConfiguration::ConfigureJob(AbstractJob *job,
                                                  ConfigurationObject *confObject,
                                                  std::vector<wstring> &errorMessages)
{
    DiskRelatedJobConfiguration::ConfigureJob(job, confObject, errorMessages);

    const wstring localTarget = confObject->GetFirstProperty(TargetProperty, L"param1");

    LinuxFreeSpaceCheckJob* castJob = static_cast<LinuxFreeSpaceCheckJob*>(job);
    if (localTarget == L"false")
        castJob->SetTargetToLocal(false);
}

void DiskSpaceCheckJobConfiguration::FillKnownProperties(std::vector<wstring> &properties)
{
    DiskRelatedJobConfiguration::FillKnownProperties(properties);
    properties.push_back(TargetProperty);
}

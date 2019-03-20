#include "diskspacecheckjobconfiguration.h"
#include "linuxfreespacecheckjob.h"

using namespace std;

const wstring DiskSpaceCheckJobConfiguration::DriveProperty = L"drive";
const wstring DiskSpaceCheckJobConfiguration::TargetProperty = L"localTarget";

DiskSpaceCheckJobConfiguration::DiskSpaceCheckJobConfiguration()
    : AbstractJobDefaultConfiguration(L"DiskSpaceCheck")
{
}

bool DiskSpaceCheckJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<LinuxFreeSpaceCheckJob*>(job) != NULL);
}

ConfigurationObject* DiskSpaceCheckJobConfiguration::CreateConfigurationObject(
      AbstractJob* job)
{
   ConfigurationObject* confObject = AbstractJobDefaultConfiguration::CreateConfigurationObject(job);
   LinuxFreeSpaceCheckJob* castJob = static_cast<LinuxFreeSpaceCheckJob*>(job);

   confObject->SetProperty(DriveProperty, castJob->GetDrive());
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
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    const wstring drive = confObject->GetFirstProperty(DriveProperty, L"param0");
    const wstring localTarget = confObject->GetFirstProperty(TargetProperty, L"param1");

    LinuxFreeSpaceCheckJob* castJob = static_cast<LinuxFreeSpaceCheckJob*>(job);
    castJob->SetDrive(drive);
    if (localTarget == L"false")
        castJob->SetTargetToLocal(false);
}

void DiskSpaceCheckJobConfiguration::FillKnownProperties(std::vector<wstring> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(DriveProperty);
    properties.push_back(TargetProperty);
}

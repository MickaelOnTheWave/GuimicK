#include "diskrelatedjobconfiguration.h"

#include "diskrelatedjob.h"

using namespace std;

const wstring DiskRelatedJobConfiguration::DriveProperty = L"drive";

DiskRelatedJobConfiguration::DiskRelatedJobConfiguration(const wstring &tag)
    : AbstractJobDefaultConfiguration(tag)
{
}

bool DiskRelatedJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<DiskRelatedJob*>(job) != NULL);
}

ConfigurationObject* DiskRelatedJobConfiguration::CreateConfigurationObject(
      AbstractJob* job)
{
   ConfigurationObject* confObject = AbstractJobDefaultConfiguration::CreateConfigurationObject(job);
   DiskRelatedJob* castJob = static_cast<DiskRelatedJob*>(job);

   confObject->SetProperty(DriveProperty, castJob->GetDrive());
   return confObject;
}

void DiskRelatedJobConfiguration::ConfigureJob(AbstractJob *job,
                                                  ConfigurationObject *confObject,
                                                  std::vector<wstring> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    const wstring drive = confObject->GetFirstProperty(DriveProperty, L"param0");

    DiskRelatedJob* castJob = static_cast<DiskRelatedJob*>(job);
    castJob->SetDrive(drive);
}

void DiskRelatedJobConfiguration::FillKnownProperties(std::vector<wstring> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(DriveProperty);
}

#include "diskspacecheckjobconfiguration.h"
#include "linuxfreespacecheckjob.h"

using namespace std;

const string DiskSpaceCheckJobConfiguration::DriveProperty = "drive";
const string DiskSpaceCheckJobConfiguration::TargetProperty = "localTarget";

DiskSpaceCheckJobConfiguration::DiskSpaceCheckJobConfiguration()
    : AbstractJobDefaultConfiguration("DiskSpaceCheck")
{
}

AbstractJob *DiskSpaceCheckJobConfiguration::CreateJob()
{
    return new LinuxFreeSpaceCheckJob();
}

void DiskSpaceCheckJobConfiguration::ConfigureJob(AbstractJob *job,
                                                  ConfigurationObject *confObject,
                                                  std::vector<string> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    const string drive = confObject->GetFirstProperty(DriveProperty, "param0");
    const string localTarget = confObject->GetFirstProperty(TargetProperty, "param1");

    LinuxFreeSpaceCheckJob* castJob = static_cast<LinuxFreeSpaceCheckJob*>(job);
    castJob->SetDrive(drive);
    if (localTarget == "false")
        castJob->SetTargetToLocal(false);
}

void DiskSpaceCheckJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(DriveProperty);
    properties.push_back(TargetProperty);
}

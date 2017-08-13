#include "diskspacecheckjobconfiguration.h"
#include "linuxfreespacecheckjob.h"

using namespace std;

static const string driveProperty = "drive";
static const string targetProperty = "localTarget";

DiskSpaceCheckJobConfiguration::DiskSpaceCheckJobConfiguration()
    : AbstractJobConfiguration("DiskSpaceCheck")
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
    AbstractJobConfiguration::ConfigureJob(job, confObject, errorMessages);

    const string drive = confObject->GetFirstProperty(driveProperty, "param0");
    const string localTarget = confObject->GetFirstProperty(targetProperty, "param1");

    LinuxFreeSpaceCheckJob* castJob = static_cast<LinuxFreeSpaceCheckJob*>(job);
    castJob->SetDrive(drive);
    if (localTarget == "false")
        castJob->SetTargetToRemote(true);
}

void DiskSpaceCheckJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    properties.push_back(driveProperty);
    properties.push_back(targetProperty);
}

#include "diskspacecheckjobconfiguration.h"
#include "linuxfreespacecheckjob.h"

using namespace std;

DiskSpaceCheckJobConfiguration::DiskSpaceCheckJobConfiguration()
    : AbstractJobConfiguration("DiskSpaceCheck")
{
}

AbstractJob *DiskSpaceCheckJobConfiguration::CreateConfiguredJobAfterCheck(
                                                ConfigurationObject *confObject,
                                                std::vector<std::string> &)
{
    const string drive = confObject->GetFirstProperty("drive", "param0");
    const string localTarget = confObject->GetFirstProperty("localTarget", "param1");

    LinuxFreeSpaceCheckJob* job = new LinuxFreeSpaceCheckJob(drive);
    if (localTarget == "false")
        job->SetTargetToRemote(true);

    return job;
}

void DiskSpaceCheckJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    properties.push_back("drive");
    properties.push_back("localTarget");
}

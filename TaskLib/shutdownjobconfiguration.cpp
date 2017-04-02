#include "shutdownjobconfiguration.h"
#include "linuxshutdownjob.h"

ShutdownJobConfiguration::ShutdownJobConfiguration()
    : AbstractJobConfiguration("Shutdown")
{
}

AbstractJob *ShutdownJobConfiguration::CreateConfiguredJobAfterCheck(
                                            ConfigurationObject *confObject,
                                            std::vector<std::string> &)
{
    LinuxShutdownJob* job = new LinuxShutdownJob();
    std::string param = confObject->propertyList["param0"];
    if (param == "showDebugInformation")
        job->SetOutputDebugInformation(true);
    return job;
}

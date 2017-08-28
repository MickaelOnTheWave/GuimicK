#include "shutdownjobconfiguration.h"
#include "linuxshutdownjob.h"

ShutdownJobConfiguration::ShutdownJobConfiguration()
    : AbstractJobDefaultConfiguration("Shutdown")
{
}

AbstractJob *ShutdownJobConfiguration::CreateJob()
{
    return new LinuxShutdownJob();
}

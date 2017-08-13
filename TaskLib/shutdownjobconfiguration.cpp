#include "shutdownjobconfiguration.h"
#include "linuxshutdownjob.h"

ShutdownJobConfiguration::ShutdownJobConfiguration()
    : AbstractJobConfiguration("Shutdown")
{
}

AbstractJob *ShutdownJobConfiguration::CreateJob()
{
    return new LinuxShutdownJob();
}

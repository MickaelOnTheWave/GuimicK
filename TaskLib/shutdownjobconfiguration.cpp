#include "shutdownjobconfiguration.h"
#include "linuxshutdownjob.h"

ShutdownJobConfiguration::ShutdownJobConfiguration()
    : AbstractJobDefaultConfiguration("Shutdown")
{
}

bool ShutdownJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<LinuxShutdownJob*>(job) != NULL);
}

AbstractJob *ShutdownJobConfiguration::CreateJob()
{
    return new LinuxShutdownJob();
}

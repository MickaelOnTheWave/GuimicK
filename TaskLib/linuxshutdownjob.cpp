#include "linuxshutdownjob.h"

LinuxShutdownJob::LinuxShutdownJob()
    : SshConsoleJob("Shutdown", "shutdown", "-h 1")
{
}

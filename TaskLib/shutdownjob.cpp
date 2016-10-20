#include "shutdownjob.h"

ShutdownJob::ShutdownJob()
    : SshConsoleJob("Shutdown", "shutdown.exe", "-f -s")
{
}

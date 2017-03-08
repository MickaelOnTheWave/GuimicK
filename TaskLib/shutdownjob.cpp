#include "shutdownjob.h"

WindowsShutdownJob::WindowsShutdownJob()
    : SshConsoleJob("Shutdown", "shutdown.exe -f -s")
{
}

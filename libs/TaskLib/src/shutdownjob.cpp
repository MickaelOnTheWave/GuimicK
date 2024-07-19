#include "shutdownjob.h"

WindowsShutdownJob::WindowsShutdownJob()
    : SshConsoleJob(L"Shutdown", L"shutdown.exe -f -s")
{
}

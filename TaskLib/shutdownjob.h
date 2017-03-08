#ifndef SHUTDOWNJOB_H
#define SHUTDOWNJOB_H

#include "sshconsolejob.h"

class WindowsShutdownJob : public SshConsoleJob
{
public:
    WindowsShutdownJob();
};

#endif // SHUTDOWNJOB_H

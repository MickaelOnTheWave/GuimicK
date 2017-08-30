#ifndef LINUXSHUTDOWNJOB_H
#define LINUXSHUTDOWNJOB_H

#include "abstractjob.h"
#include "sshconsolejob.h"

class LinuxShutdownJob : public AbstractJob
{
public:
    LinuxShutdownJob();
    virtual ~LinuxShutdownJob();

    virtual std::string GetName();

    virtual AbstractJob *Clone();

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

private:
    /**
     * @return number of seconds elapsed until computer is down
     */
    int WaitForComputerToGoDown() const;

    SshConsoleJob* shutdownJob;
    std::string computer;
    int jobTimeoutInSeconds;
};

#endif // LINUXSHUTDOWNJOB_H

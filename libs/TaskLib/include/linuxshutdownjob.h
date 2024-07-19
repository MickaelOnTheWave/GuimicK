#ifndef LINUXSHUTDOWNJOB_H
#define LINUXSHUTDOWNJOB_H

#include "abstractjob.h"
#include "sshconsolejob.h"

class LinuxShutdownJob : public AbstractJob
{
public:
    LinuxShutdownJob();
    LinuxShutdownJob(const LinuxShutdownJob& other);
    virtual ~LinuxShutdownJob();

    virtual AbstractJob *Clone();

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    int GetTimeout() const;
    void SetTimeout(const int value);

private:
    /**
     * @return number of seconds elapsed until computer is down
     */
    int WaitForComputerToGoDown() const;

    bool IsShutdownOk(JobStatus* status) const;

    SshConsoleJob* shutdownJob;
    std::wstring computer;
    int jobTimeoutInSeconds;
};

#endif // LINUXSHUTDOWNJOB_H

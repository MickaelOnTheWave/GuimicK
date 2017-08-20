#ifndef WAKEJOB_H
#define WAKEJOB_H

#include "abstractjob.h"

class WakeJob : public AbstractJob
{
public:
    WakeJob();
    WakeJob(const WakeJob& other);
    virtual ~WakeJob();

    virtual std::string GetName();

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

private:

    bool HasMandatoryParameters(void) const;
    /**
     * @return number of seconds elapsed until computer is up
     */
    int WaitForComputerToGoUp(void) const;

    std::string macAddress;
    std::string broadcastIp;
    std::string expectedIp;
    std::string wakelanPath;
};

#endif // WAKEJOB_H

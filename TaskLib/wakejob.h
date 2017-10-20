#ifndef WAKEJOB_H
#define WAKEJOB_H

#include "abstractjob.h"

class WakeJob : public AbstractJob
{
public:
    WakeJob();
    WakeJob(const WakeJob& other);
    virtual ~WakeJob();

    virtual AbstractJob* Clone();
    virtual bool InitializeFromClient(Client* client);
    virtual bool IsInitialized(void);
    virtual JobStatus* Run();

    int GetTimeout() const;
    void SetTimeout(const int value);

    int GetMaxRetries() const;
    void SetMaxRetries(const int value);

private:

    bool HasMandatoryParameters(void) const;
    /**
     * @return number of seconds elapsed until computer is up
     */
    int WaitForComputerToGoUp(void) const;

    std::string macAddress;
    std::string broadcastIp;
    std::string expectedIp;
    int timeout;
    int maxRetries;
};

#endif // WAKEJOB_H

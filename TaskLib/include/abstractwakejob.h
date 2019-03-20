#ifndef ABSTRACTWAKEJOB_H
#define ABSTRACTWAKEJOB_H

#include "abstractjob.h"

class AbstractWakeJob : public AbstractJob
{
public:
    AbstractWakeJob();
    AbstractWakeJob(const AbstractWakeJob& other);
    virtual ~AbstractWakeJob();

    virtual bool InitializeFromClient(Client* client);
    virtual bool IsInitialized(void);
    virtual JobStatus* Run();

    int GetTimeout() const;
    void SetTimeout(const int value);

    int GetMaxRetries() const;
    void SetMaxRetries(const int value);

protected:
    std::wstring macAddress;
    std::wstring broadcastIp;

private:
    virtual JobStatus* SetupWaker() = 0;
    virtual JobStatus* RunWaker() = 0;

    bool HasMandatoryParameters(void) const;
    /**
     * @return number of seconds elapsed until computer is up
     */
    int WaitForComputerToGoUp(void) const;

    std::wstring expectedIp;
    int timeout;
    int maxRetries;
};

#endif // ABSTRACTWAKEJOB_H

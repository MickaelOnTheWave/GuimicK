#ifndef WAKEJOB_H
#define WAKEJOB_H

#include "abstractjob.h"

class WakeJob : public AbstractJob
{
public:
    WakeJob();    
    virtual ~WakeJob();


    virtual std::string GetName();

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

    void SetOutputDebugInformation(const bool value);

    virtual JobStatus* Run();

private:
    /**
     * @return number of seconds elapsed until computer is up
     */
    int WaitForComputerToGoUp(void) const;

    JobStatus* CreateStatus(const int code,
                            const std::string& description,
                            const std::string& debugInformation) const;

    std::string CreateValueInformationLine(
            const std::string& label,
            const int value) const;

    std::string macAddress;
    std::string broadcastIp;
    std::string expectedIp;
    bool outputDebugInformation;
};

#endif // WAKEJOB_H

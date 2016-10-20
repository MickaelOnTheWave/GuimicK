#ifndef CONSOLEWAKEJOB_H
#define CONSOLEWAKEJOB_H

#include "consolejob.h"

#include <string>

class ConsoleWakeJob : public ConsoleJob
{
public:
	ConsoleWakeJob();

    void SetTarget(const std::string& _macAddress, const std::string& _broadcastIp,
                   const std::string& _expectedIp);

	virtual bool InitializeFromClient(Client *client);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

protected:
    virtual std::string CreateFullCommand(void);

	std::string macAddress;
	std::string broadcastIp;
	std::string expectedIp;
};

#endif // CONSOLEWAKEJOB_H

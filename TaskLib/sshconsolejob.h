#ifndef SSHCONSOLEJOB_H
#define SSHCONSOLEJOB_H

#include "consolejob.h"

class SshConsoleJob : public ConsoleJob
{
public:
    SshConsoleJob(const std::string& _commandTitle, const std::string& _commandName = "",
                    const std::string& _commandParameters = "", int _expectedReturnCode = 0);

    void SetTarget(const std::string& _user, const std::string& _host);

    virtual bool InitializeFromClient(Client *client);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

protected:
    virtual std::string CreateFullCommand(void);

	std::string user;
	std::string host;
};

#endif // SSHCONSOLEJOB_H

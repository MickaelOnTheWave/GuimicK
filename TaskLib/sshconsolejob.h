#ifndef SSHCONSOLEJOB_H
#define SSHCONSOLEJOB_H

#include "userconsolejob.h"

class SshConsoleJob : public UserConsoleJob
{
public:
    SshConsoleJob(const std::string& _commandTitle, const std::string& _command = "",
                    int _expectedReturnCode = 0);
    SshConsoleJob(const SshConsoleJob& other);

    virtual AbstractJob* Clone();

    void SetTarget(const std::string& _user, const std::string& _host);

    virtual bool InitializeFromClient(Client *client);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

private:
	std::string user;
	std::string host;
};

#endif // SSHCONSOLEJOB_H

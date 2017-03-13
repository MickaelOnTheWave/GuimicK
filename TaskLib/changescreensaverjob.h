#ifndef CHANGESCREENSAVERJOB_H
#define CHANGESCREENSAVERJOB_H

#include <string>

#include "userconsolejob.h"

class ChangeScreensaverJob : public UserConsoleJob
{
public:
	ChangeScreensaverJob(int _timeOut);

	virtual bool InitializeFromClient(Client *client);

    virtual bool IsInitialized(void);

protected:
	std::string sshUser;
	std::string clientIp;
	int timeOut;
};

#endif // CHANGESCREENSAVERJOB_H

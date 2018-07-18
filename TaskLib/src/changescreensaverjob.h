#ifndef CHANGESCREENSAVERJOB_H
#define CHANGESCREENSAVERJOB_H

#include <string>

#include "userconsolejob.h"

class ChangeScreensaverJob : public UserConsoleJob
{
public:
    ChangeScreensaverJob(int _timeOut = 0);

	virtual bool InitializeFromClient(Client *client);

    virtual bool IsInitialized(void);

    int GetTimeout(void) const;
    void SetTimeout(const int value);

protected:
	std::string sshUser;
	std::string clientIp;
	int timeOut;
};

#endif // CHANGESCREENSAVERJOB_H

#include "changescreensaverjob.h"

#include <sstream>

using namespace std;

ChangeScreensaverJob::ChangeScreensaverJob(int _timeOut)
    : UserConsoleJob("Change Screen Saver", ""),
	  sshUser(""), clientIp(""), timeOut(_timeOut)
{
}

bool ChangeScreensaverJob::InitializeFromClient(Client *client)
{
	if (!client->HasProperty("sshuser"))
		return false;
	else if (!client->HasProperty("ip"))
		return false;

	sshUser = client->GetProperty("sshuser");
	clientIp = client->GetProperty("ip");

	stringstream strTimeout;
	strTimeout << timeOut;

    command = "sh changeComputerScreensaver.sh -u remotebackup -h desktop -t 3600";
	/*commandName += " -u " + sshUser;
	commandName += " -h " + clientIp;
	commandName += " -t " + strTimeout.str();*/

/*	commandName = "ssh remotebackup@desktop";
commandName += $USER@$HOST << 'ENDSSH'
												$SETSCRTIMEOUT_BIN $TIMEOUT_SEC
												ENDSSH*/
    return true;
}

bool ChangeScreensaverJob::IsInitialized()
{
    return true;
}

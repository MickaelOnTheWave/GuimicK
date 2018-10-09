#include "remoteclientconfiguration.h"

using namespace std;

bool RemoteClientConfiguration::AreClientPropertiesConsistent(
      ConfigurationObject* object, std::vector<std::string>& errorMessages
      )
{
	if (!LocalClientConfiguration::AreClientPropertiesConsistent(object, errorMessages))
		return false;
	if (client->GetProperty("ip") == "")
	{
	  errorMessages.push_back("Error : Client without IP address");
	  return false;
	}
	else if (client->GetProperty("sshuser") == "")
	{
	  errorMessages.push_back("Error : Client without Ssh user");
	  return false;
	}
	else
	  return true;
}

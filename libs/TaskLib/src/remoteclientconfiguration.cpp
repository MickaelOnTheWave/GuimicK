#include "remoteclientconfiguration.h"

using namespace std;

bool RemoteClientConfiguration::AreClientPropertiesConsistent(
      ConfigurationObject* object, vector<wstring>& errorMessages
      )
{
	if (!LocalClientConfiguration::AreClientPropertiesConsistent(object, errorMessages))
		return false;
	if (client->GetProperty(L"ip") == L"")
	{
	  errorMessages.push_back(L"Error : Client without IP address");
	  return false;
	}
	else if (client->GetProperty(L"sshuser") == L"")
	{
	  errorMessages.push_back(L"Error : Client without Ssh user");
	  return false;
	}
	else
	  return true;
}

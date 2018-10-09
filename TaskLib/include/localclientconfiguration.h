#ifndef LOCALCLIENTCONFIGURATION_H
#define LOCALCLIENTCONFIGURATION_H

#include <vector>

#include "client.h"
#include "configurationobject.h"

class LocalClientConfiguration
{
public:
	static std::string MsgClientWithoutName;
	static std::string MsgClientWithoutJobs;
	static std::string MsgDeprecatedRemoteOption;

	Client* CreateConfiguredClient(ConfigurationObject* confObject,
	                               std::vector<std::string> &errorMessages);

protected:
   virtual bool AreClientPropertiesConsistent(ConfigurationObject* object,
                                              std::vector<std::string> &errorMessages);

   Client* client;

private:
	void SetAllClientProperties(ConfigurationObject* object);
};

#endif // LOCALCLIENTCONFIGURATION_H

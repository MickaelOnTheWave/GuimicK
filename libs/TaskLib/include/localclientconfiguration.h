#ifndef LOCALCLIENTCONFIGURATION_H
#define LOCALCLIENTCONFIGURATION_H

#include <vector>

#include "client.h"
#include "configurationobject.h"

class LocalClientConfiguration
{
public:
	static std::wstring MsgClientWithoutName;
	static std::wstring MsgClientWithoutJobs;
	static std::wstring MsgDeprecatedRemoteOption;

	Client* CreateConfiguredClient(ConfigurationObject* confObject,
	                               std::vector<std::wstring> &errorMessages);

protected:
   virtual bool AreClientPropertiesConsistent(ConfigurationObject* object,
                                              std::vector<std::wstring> &errorMessages);

   Client* client;

private:
	void SetAllClientProperties(ConfigurationObject* object);
};

#endif // LOCALCLIENTCONFIGURATION_H

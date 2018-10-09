#ifndef REMOTECLIENTCONFIGURATION_H
#define REMOTECLIENTCONFIGURATION_H

#include "localclientconfiguration.h"

class RemoteClientConfiguration : public LocalClientConfiguration
{
protected:
	virtual bool AreClientPropertiesConsistent(ConfigurationObject* object,
	                                           std::vector<std::string> &errorMessages);

};

#endif // REMOTECLIENTCONFIGURATION_H

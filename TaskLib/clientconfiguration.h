#ifndef CLIENTCONFIGURATION_H
#define CLIENTCONFIGURATION_H

#include <vector>

#include "client.h"
#include "configurationobject.h"

class ClientConfiguration
{
public:
   static std::string MsgClientWithoutName;
   static std::string MsgClientWithoutJobs;
   static std::string MsgDeprecatedRemoteOption;

   ClientConfiguration();

   Client* CreateConfiguredClient(ConfigurationObject* confObject,
                                  std::vector<std::string> &errorMessages);

private:
   bool AreClientPropertiesConsistent(ConfigurationObject* object,
                                      std::vector<std::string> &errorMessages);

   Client* client;
};

#endif // CLIENTCONFIGURATION_H

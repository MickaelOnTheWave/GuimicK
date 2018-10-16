#include "serverconfiguration.h"

#include <vector>

#include "configurationparser.h"
#include "consolejob.h"

#include "tools.h"

#include "changescreensaverjobconfiguration.h"
#include "clamavjobconfiguration.h"
#include "configurationtools.h"
#include "copyfsbackupjobconfigurations.h"
#include "diskspacecheckjobconfiguration.h"
#include "gitbackupjobconfiguration.h"
#include "gitfsbackupjobconfiguration.h"
#include "remoteclientconfiguration.h"
#include "rsnapshotbackupjobconfiguration.h"
#include "shutdownjobconfiguration.h"
#include "sshconsolejobconfiguration.h"
#include "userconsolejobconfiguration.h"
#include "wakejobconfiguration.h"

using namespace std;

string ServerConfiguration::MsgNoPassword = "Client requires password";
string ServerConfiguration::MsgNoConfigFile = "Client configuration file missing";
string ServerConfiguration::MsgClientConfigAccessError = "Error trying to access Client configuration";
string ServerConfiguration::MsgClientConfigUnknownObjects = "Client configuration has unknown objects";
string ServerConfiguration::MsgClientConfigEmpty = "Client configuration is empty";
string ServerConfiguration::MsgRemoteOptionDeprecated = "Remote option deprecated";

ServerConfiguration::ServerConfiguration()
    : StandaloneConfiguration()
{
}

ServerConfiguration::~ServerConfiguration()
{
}

ConfigurationType ServerConfiguration::GetType() const
{
   return ServerConfigurationType;
}

void ServerConfiguration::CreateAgent(ConfigurationObject* confObject,
                                      std::vector<string>& errorMessages)
{
   StandaloneConfiguration::CreateAgent(confObject, errorMessages);

   // TODO : think about the proper place for that
   ConfigurationObject* pathsObject = confObject->GetObject("DefaultBinPaths");
   if (pathsObject)
   {
       map<string, string>::iterator itPath = pathsObject->propertyList.begin();
       map<string, string>::iterator endPath = pathsObject->propertyList.end();
       for (; itPath != endPath; itPath++)
       {
           pair<string, string> currentPathPair = *itPath;
           ConsoleJob::AddAppSearchPath(currentPathPair.second);
       }
   }

}

bool ServerConfiguration::CreateClient(ConfigurationObject* confObject,
                                       std::vector<string>& errorMessages)
{
   if (client != NULL)
   {
      // One day, this condition will have to be removed... a server
      // is meant to manage multiple clients!
      errorMessages.push_back(ConfigurationTools::CreateWarning(MsgOneClientSupported));
      delete client;
   }

   RemoteClientConfiguration config;
   client = config.CreateConfiguredClient(confObject, errorMessages);
   return (client != NULL);
}


#include "serverconfiguration.h"

#include <vector>

#include "configurationparser.h"
#include "consolejob.h"

#include "tools.h"

#include "clamavjobconfiguration.h"
#include "configurationtools.h"
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

wstring ServerConfiguration::MsgNoPassword                  = L"Client requires password";
wstring ServerConfiguration::MsgNoConfigFile                = L"Client configuration file missing";
wstring ServerConfiguration::MsgClientConfigAccessError     = L"Error trying to access Client configuration";
wstring ServerConfiguration::MsgClientConfigUnknownObjects  = L"Client configuration has unknown objects";
wstring ServerConfiguration::MsgClientConfigEmpty           = L"Client configuration is empty";
wstring ServerConfiguration::MsgRemoteOptionDeprecated      = L"Remote option deprecated";

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

AbstractTypeConfiguration* ServerConfiguration::Copy() const
{
   return new ServerConfiguration();
}

void ServerConfiguration::CreateAgent(ConfigurationObject* confObject,
                                      std::vector<wstring>& errorMessages)
{
   StandaloneConfiguration::CreateAgent(confObject, errorMessages);

   // TODO : think about the proper place for that
   ConfigurationObject* pathsObject = confObject->GetObject(L"DefaultBinPaths");
   if (pathsObject)
   {
       map<wstring, wstring>::iterator itPath = pathsObject->propertyList.begin();
       map<wstring, wstring>::iterator endPath = pathsObject->propertyList.end();
       for (; itPath != endPath; itPath++)
       {
           pair<wstring, wstring> currentPathPair = *itPath;
           ConsoleJob::AddAppSearchPath(currentPathPair.second);
       }
   }

}

bool ServerConfiguration::CreateClient(ConfigurationObject* confObject,
                                       std::vector<wstring>& errorMessages)
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


#ifndef SERVERCONFIGURATION_H
#define SERVERCONFIGURATION_H

#include <vector>
#include <string>

#include "client.h"
#include "standaloneconfiguration.h"
#include "textreportcreator.h"

class ServerConfiguration : public StandaloneConfiguration
{
public:
    static std::string MsgNoPassword;
    static std::string MsgNoConfigFile;
    static std::string MsgClientConfigAccessError;
    static std::string MsgClientConfigUnknownObjects;
    static std::string MsgClientConfigEmpty;
    static std::string MsgRemoteOptionDeprecated;

    ServerConfiguration();
    virtual ~ServerConfiguration();

protected:
    virtual void CreateAgent(ConfigurationObject* confObject,
                             std::vector<std::string>& errorMessages);
    virtual bool CreateClient(ConfigurationObject* confObject,
                              std::vector<std::string> &errorMessages);
};

#endif // SERVERCONFIGURATION_H

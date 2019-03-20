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
    static std::wstring MsgNoPassword;
    static std::wstring MsgNoConfigFile;
    static std::wstring MsgClientConfigAccessError;
    static std::wstring MsgClientConfigUnknownObjects;
    static std::wstring MsgClientConfigEmpty;
    static std::wstring MsgRemoteOptionDeprecated;

    ServerConfiguration();
    virtual ~ServerConfiguration();

    virtual ConfigurationType GetType() const;

    virtual AbstractTypeConfiguration* Copy() const;

protected:
    virtual void CreateAgent(ConfigurationObject* confObject,
                             std::vector<std::wstring>& errorMessages);
    virtual bool CreateClient(ConfigurationObject* confObject,
                              std::vector<std::wstring> &errorMessages);
};

#endif // SERVERCONFIGURATION_H

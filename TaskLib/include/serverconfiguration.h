#ifndef SERVERCONFIGURATION_H
#define SERVERCONFIGURATION_H

#include <vector>
#include <string>

#include "AbstractReportDispatcher.h"
#include "client.h"
#include "SelfIdentity.h"
#include "taskmanagerconfiguration.h"
#include "textreportcreator.h"

class ServerConfiguration : public TaskManagerConfiguration
{
public:
    static std::string MsgNoPassword;
    static std::string MsgNoConfigFile;
    static std::string MsgClientConfigAccessError;
    static std::string MsgClientConfigUnknownObjects;
    static std::string MsgClientConfigEmpty;
    static std::string MsgMissingClient;
    static std::string MsgMissingAgent;
    static std::string MsgRemoteOptionDeprecated;
    static std::string MsgOneClientSupported;

    ServerConfiguration();
    virtual ~ServerConfiguration();

    virtual ClientWorkManager* BuildWorkList(const bool withProfiling) const;

    AbstractReportCreator* GetReportCreator(void) const;

    AbstractReportDispatcher* CreateReportDispatcher(const bool commandLinePreventsEmail) const;

    const SelfIdentity *GetAgent() const;
    void SetAgent(SelfIdentity *agent);

	std::string GetMasterEmail() const;

	bool GetLocalShutdown() const;

    std::string GetReportDispatching() const;

    bool IsReportHtml(void) const;

private:
    virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::string> &errorMessages);
    virtual void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::string> &errorMessages);
    virtual bool IsConfigurationConsistent(std::vector<std::string> &errorMessages);

    virtual void SaveContentToOpenedFile(std::ofstream& file);

    void SaveAgentToOpenedFile(std::ofstream& file);
    void SaveClientToOpenedFile(std::ofstream& file);
    void SaveReportOptionsToOpenedFile(std::ofstream& file);
    void SaveGlobalPropertiesToOpenedFile(std::ofstream& file);

    bool IsEmailDataComplete() const;

    bool CreateClient(ConfigurationObject* confObject, std::vector<std::string> &errorMessages);
    void CreateAgent(ConfigurationObject* confObject, std::vector<std::string>& errorMessages);
    void CreateReport(ConfigurationObject* confObject, std::vector<std::string>& errorMessages);

    AbstractReportCreator* CreateReportObject(const std::string& type) const;

    bool GetBooleanValue(const std::string& strValue, std::vector<std::string>& errorMessages) const;

    void SaveStringValueToFile(std::ofstream& file,
                               const std::string& name, const std::string& value);

	SelfIdentity* self;
   AbstractReportCreator* reportCreator;
   std::string reportType;
   std::string cssFile;
	std::string masterEmail;
   std::string reportDispatching;
	bool shutdown;
};

#endif // SERVERCONFIGURATION_H

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <vector>
#include <string>

#include "abstractjobconfiguration.h"
#include "AbstractReportDispatcher.h"
#include "client.h"
#include "SelfIdentity.h"
#include "taskmanagerconfiguration.h"
#include "textreportcreator.h"

class Configuration : public TaskManagerConfiguration
{
public:
    static std::string MsgNoPassword;
    static std::string MsgNoConfigFile;
    static std::string MsgClientConfigAccessError;
    static std::string MsgClientConfigUnknownObjects;
    static std::string MsgClientConfigEmpty;
    static std::string MsgMissingClient;
    static std::string MsgMissingAgent;
    static std::string MsgClientWithoutName;
    static std::string MsgRemoteOptionDeprecated;
    static std::string MsgOneClientSupported;

	Configuration();
	virtual ~Configuration();

    bool LoadFromFile(const std::string& fileName, std::vector<std::string> &errorMessages);

    AbstractReportCreator* GetReportCreator(void) const;

    AbstractReportDispatcher* CreateReportDispatcher(const bool commandLinePreventsEmail) const;

    const SelfIdentity *GetAgent() const;
    Client* GetClient();

	std::string GetMasterEmail() const;

	bool GetLocalShutdown() const;

    std::string GetReportDispatching() const;

    bool IsReportHtml(void) const;

    bool HasClient() const;

private:
    void FillSupportedJobsList();

    virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::string> &errorMessages);
    virtual void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::string> &errorMessages);
    virtual bool IsConfigurationConsistent(std::vector<std::string> &errorMessages);

    bool IsEmailDataComplete() const;

    AbstractJob *CreateJobFromObject(ConfigurationObject *object,
                                     std::vector<std::string> &errorMessages);

    bool CreateClient(ConfigurationObject* confObject, std::vector<std::string> &errorMessages);
    void CreateAgent(ConfigurationObject* confObject, std::vector<std::string>& errorMessages);
    void CreateReport(ConfigurationObject* confObject, std::vector<std::string>& errorMessages);

    AbstractReportCreator* CreateReportObject(const std::string& type) const;

    bool GetBooleanValue(const std::string& strValue, std::vector<std::string>& errorMessages) const;

    AbstractJobConfiguration* GetJobConfiguration(const std::string& jobTab);

    bool AreClientPropertiesConsistent(ConfigurationObject* object,
                                       std::vector<std::string> &errorMessages);

    bool FillJobListLocally(ConfigurationObject* jobListObj,
                            std::vector<std::string> &errorMessages);
    bool FillJobListRemotely(Client *client, std::vector<std::string> &errorMessages);

    bool CopyClientFile(Client *client,
                        const std::string& source, const std::string& destination,
                        std::vector<std::string>& errorMessages);
    void FillRemoteClientObjects(const std::list<ConfigurationObject*>& objectList,
                                 std::vector<std::string> &errorMessages);

    std::string CreateScpErrorMessage(const std::string& output) const;

    std::string CreateWarning(const std::string& message) const;
    std::string CreateError(const std::string& message) const;
    std::string CreateMessage(const std::string& tag, const std::string& message) const;

	SelfIdentity* self;
    std::vector<AbstractJobConfiguration*> supportedJobs;

    AbstractReportCreator* reportCreator;

	std::string masterEmail;
    std::string reportDispatching;
	bool shutdown;
};

#endif // CONFIGURATION_H

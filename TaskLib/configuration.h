#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <vector>
#include <string>

#include "abstractjobconfiguration.h"
#include "client.h"
#include "clientworkmanager.h"
#include "SelfIdentity.h"
#include "textreportcreator.h"

class ConfigurationObject;

class Configuration
{
public:
	Configuration();
	virtual ~Configuration();

    bool LoadFromFile(const std::string& fileName, std::vector<std::string> &errorMessages);

    ClientWorkManager* BuildTimedWorkList() const;

    ClientWorkManager* BuildSimpleWorkList() const;

    AbstractReportCreator* GetReportCreator(void) const;

    SelfIdentity* GetAgent();
    Client* GetClient();

	std::string GetMasterEmail() const;

	bool GetLocalShutdown() const;

	bool GetSendReportByEmail() const;

    bool IsReportHtml(void) const;

    bool HasClient() const;

private:
    void FillSupportedJobsList();

    void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::string> &errorMessages);
    void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::string> &errorMessages);
    bool IsConfigurationConsistent(std::vector<std::string> &errorMessages);
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
    bool FillJobListRemotely(std::vector<std::string> &errorMessages);

    void CopyClientFile(const std::string& source, const std::string& destination);
    void FillRemoteClientObjects(const std::list<ConfigurationObject*>& objectList,
                                 std::vector<std::string> &errorMessages);

	Client* client;
	SelfIdentity* self;
    std::list<AbstractJob*> jobList;
    std::vector<AbstractJobConfiguration*> supportedJobs;

    AbstractReportCreator* reportCreator;

	std::string masterEmail;
	bool emailReport;
	bool shutdown;
    bool hasFatalError;
};

#endif // CONFIGURATION_H

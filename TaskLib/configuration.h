#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <list>
#include <string>

#include "client.h"
#include "clientworkmanager.h"
#include "consolewakejob.h"
#include "shutdownjob.h"
#include "changescreensaverjob.h"
#include "clamavjob.h"
#include "backupjob.h"
#include "textreportcreator.h"
#include "gitbackupjob.h"
#include "SelfIdentity.h"

class ConfigurationObject;

class Configuration
{
public:
	Configuration();
	virtual ~Configuration();

	bool LoadFromFile(const std::string& fileName, std::list<std::string> &errorMessages);

	ClientWorkManager* BuildWorkList();

	AbstractReportCreator* CreateReportObject();

	SelfIdentity* GetSelfIdentity();

	std::string GetMasterEmail() const;

	bool GetLocalShutdown() const;

	bool GetSendReportByEmail() const;

    bool IsHtmlReport() const;

protected:

	AbstractJob *CreateJobFromObject(ConfigurationObject *object);
    void InitializeConsoleJobFromObject(ConfigurationObject *object, ConsoleJob* job);

    GitBackupJob* CreateGitBackupJob(ConfigurationObject* object);

	void CreateClient(ConfigurationObject* confObject, std::list<std::string>& errorMessages);
	void CreateSelf(ConfigurationObject* confObject, std::list<std::string>& errorMessages);

	int GetReportType(const std::string& strType) const;
	bool GetBooleanValue(const std::string& strValue, std::list<std::string>& errorMessages) const;

	Client* client;
	SelfIdentity* self;
	std::list<AbstractJob*> jobList;

	static const int TEXT_REPORT = 0;
	static const int HTML_REPORT = 1;

	int reportType;
	std::string masterEmail;
	bool emailReport;
	bool shutdown;
};

#endif // CONFIGURATION_H

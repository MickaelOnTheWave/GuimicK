#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <list>
#include <string>

#include "client.h"
#include "clientworkmanager.h"
#include "SelfIdentity.h"
#include "textreportcreator.h"

#include "changescreensaverjob.h"
#include "consolejob.h"
#include "gitbackupjob.h"
#include "linuxshutdownjob.h"
#include "rsnapshotbackupjob.h"
#include "wakejob.h"

class ConfigurationObject;

class Configuration
{
public:
	Configuration();
	virtual ~Configuration();

	bool LoadFromFile(const std::string& fileName, std::list<std::string> &errorMessages);

    ClientWorkManager* BuildWorkList() const;

    ClientWorkManager* BuildSimpleWorkList() const;

    AbstractReportCreator* CreateReportObject() const;

	SelfIdentity* GetSelfIdentity();

	std::string GetMasterEmail() const;

	bool GetLocalShutdown() const;

	bool GetSendReportByEmail() const;

    bool IsHtmlReport() const;

    bool HasClient() const;

protected:

	AbstractJob *CreateJobFromObject(ConfigurationObject *object);

    ConsoleJob *InitializeConsoleJobFromObject(ConfigurationObject *object, ConsoleJob* job) const;
    WakeJob* CreateWakeJobFromObject(ConfigurationObject *object) const;
    LinuxShutdownJob* CreateShutdownJobFromObject(ConfigurationObject *object) const;
    ChangeScreensaverJob* CreateChangeScreensaverJobFromObject(ConfigurationObject* object) const;
    GitBackupJob* CreateGitBackupJob(ConfigurationObject* object) const;
    RsnapshotBackupJob* CreateRsnapshotBackupJob(ConfigurationObject* object) const;

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

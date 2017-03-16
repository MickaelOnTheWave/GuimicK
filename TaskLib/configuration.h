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
#include "linuxfreespacecheckjob.h"
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

    ClientWorkManager* BuildTimedWorkList() const;

    ClientWorkManager* BuildSimpleWorkList() const;

    AbstractReportCreator* GetReportCreator(void) const;

	SelfIdentity* GetSelfIdentity();

	std::string GetMasterEmail() const;

	bool GetLocalShutdown() const;

	bool GetSendReportByEmail() const;

    bool IsHtmlReport(void) const;

    bool HasClient() const;

protected:

	AbstractJob *CreateJobFromObject(ConfigurationObject *object);

    UserConsoleJob *InitializeUserConsoleJobFromObject(ConfigurationObject *object) const;
    SshConsoleJob *InitializeSshConsoleJobFromObject(ConfigurationObject *object) const;
    WakeJob* CreateWakeJobFromObject(ConfigurationObject *object) const;
    LinuxShutdownJob* CreateShutdownJobFromObject(ConfigurationObject *object) const;
    ChangeScreensaverJob* CreateChangeScreensaverJobFromObject(ConfigurationObject* object) const;
    GitBackupJob* CreateGitBackupJob(ConfigurationObject* object) const;
    RsnapshotBackupJob* CreateRsnapshotBackupJob(ConfigurationObject* object) const;
    LinuxFreeSpaceCheckJob* CreateDiskSpaceCheckJob(ConfigurationObject* object) const;

    RsnapshotBackupJob* CreateRsnapshotBackupJobFromCreator(ConfigurationObject *object,
                                                            const std::string& repository) const;

	void CreateClient(ConfigurationObject* confObject, std::list<std::string>& errorMessages);
	void CreateSelf(ConfigurationObject* confObject, std::list<std::string>& errorMessages);
    void CreateReport(ConfigurationObject* confObject, std::list<std::string>& errorMessages);

    AbstractReportCreator* CreateReportObject(const std::string& type) const;

	bool GetBooleanValue(const std::string& strValue, std::list<std::string>& errorMessages) const;

	Client* client;
	SelfIdentity* self;
	std::list<AbstractJob*> jobList;

    AbstractReportCreator* reportCreator;

	std::string masterEmail;
	bool emailReport;
	bool shutdown;
};

#endif // CONFIGURATION_H

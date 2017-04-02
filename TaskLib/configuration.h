#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <vector>
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

    bool LoadFromFile(const std::string& fileName, std::vector<std::string> &errorMessages);

    ClientWorkManager* BuildTimedWorkList() const;

    ClientWorkManager* BuildSimpleWorkList() const;

    AbstractReportCreator* GetReportCreator(void) const;

	SelfIdentity* GetSelfIdentity();

	std::string GetMasterEmail() const;

	bool GetLocalShutdown() const;

	bool GetSendReportByEmail() const;

    bool IsReportHtml(void) const;

    bool HasClient() const;

protected:

    void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::string> &errorMessages);
    void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::string> &errorMessages);
    bool IsConfigurationConsistent(std::vector<std::string> &errorMessages);
    bool IsEmailDataComplete() const;

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

    void CreateClient(ConfigurationObject* confObject, std::vector<std::string> &errorMessages);
    void CreateSelf(ConfigurationObject* confObject, std::vector<std::string>& errorMessages);
    void CreateReport(ConfigurationObject* confObject, std::vector<std::string>& errorMessages);

    AbstractReportCreator* CreateReportObject(const std::string& type) const;

    bool GetBooleanValue(const std::string& strValue, std::vector<std::string>& errorMessages) const;

	Client* client;
	SelfIdentity* self;
	std::list<AbstractJob*> jobList;

    AbstractReportCreator* reportCreator;

	std::string masterEmail;
	bool emailReport;
	bool shutdown;
};

#endif // CONFIGURATION_H

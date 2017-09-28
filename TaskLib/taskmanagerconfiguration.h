#ifndef TASKMANAGERCONFIGURATION_H
#define TASKMANAGERCONFIGURATION_H

#include <list>

#include "abstractjob.h"
#include "abstractjobconfiguration.h"
#include "clientworkmanager.h"
#include "configurationparser.h"

class ConfigurationObject;

class TaskManagerConfiguration
{
public:
    TaskManagerConfiguration();
    virtual ~TaskManagerConfiguration();

    bool LoadFromFile(const std::string& fileName,
                      std::vector<std::string> &errorMessages);
    bool LoadFromBuffer(const std::string& content,
                         std::vector<std::string> &errorMessages);

    ClientWorkManager* BuildTimedWorkList() const;
    ClientWorkManager* BuildSimpleWorkList() const;

    bool HasClient() const;
    Client* GetClient();

protected:
    std::string CreateWarning(const std::string& message) const;
    std::string CreateError(const std::string& message) const;
    std::string CreateMessage(const std::string& tag, const std::string& message) const;

    AbstractJob *CreateJobFromObject(ConfigurationObject *object,
                                     std::vector<std::string> &errorMessages);

    Client* client;
    std::list<AbstractJob*> jobList;
    bool hasFatalError;

private:
    virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::string> &errorMessages) = 0;
    virtual void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::string> &errorMessages) = 0;
    virtual bool IsConfigurationConsistent(std::vector<std::string> &errorMessages) = 0;

    void FillSupportedJobsList();
    void FreeSupportedJobsList();

    AbstractJobConfiguration* GetJobConfiguration(const std::string& jobTab);

    void Reset();

    bool SetupData(const ConfigurationParser& parser,
                   std::vector<std::string> &errorMessages);

    std::vector<AbstractJobConfiguration*> supportedJobs;
};

#endif // TASKMANAGERCONFIGURATION_H

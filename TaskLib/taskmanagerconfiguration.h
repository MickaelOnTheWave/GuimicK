#ifndef TASKMANAGERCONFIGURATION_H
#define TASKMANAGERCONFIGURATION_H

#include <list>
#include "abstractjob.h"
#include "clientworkmanager.h"

class ConfigurationObject;

class TaskManagerConfiguration
{
public:
    TaskManagerConfiguration();

    bool LoadFromFile(const std::string& fileName,
                      std::vector<std::string> &errorMessages);

    ClientWorkManager* BuildTimedWorkList() const;
    ClientWorkManager* BuildSimpleWorkList() const;

protected:
    Client* client;
    std::list<AbstractJob*> jobList;
    bool hasFatalError;

private:
    virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::string> &errorMessages) = 0;
    virtual void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::string> &errorMessages) = 0;
    virtual bool IsConfigurationConsistent(std::vector<std::string> &errorMessages) = 0;

};

#endif // TASKMANAGERCONFIGURATION_H

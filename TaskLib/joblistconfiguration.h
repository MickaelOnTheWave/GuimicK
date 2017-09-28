#ifndef JOBLISTCONFIGURATION_H
#define JOBLISTCONFIGURATION_H

#include <string>
#include <vector>

#include "clientworkmanager.h"
#include "taskmanagerconfiguration.h"

class JobListConfiguration : public TaskManagerConfiguration
{
public:
    JobListConfiguration();

    bool Parse(const std::string& content, std::vector<std::string>& errors);

private:
    virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::string> &errorMessages);
    virtual void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::string> &errorMessages);
    virtual bool IsConfigurationConsistent(std::vector<std::string> &errorMessages);

};

#endif // JOBLISTCONFIGURATION_H

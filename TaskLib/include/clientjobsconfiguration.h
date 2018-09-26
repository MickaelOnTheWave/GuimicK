#ifndef CLIENTJOBSCONFIGURATION_H
#define CLIENTJOBSCONFIGURATION_H

#include <string>
#include <vector>

#include "clientworkmanager.h"
#include "configurationobject.h"
#include "taskmanagerconfiguration.h"

class ClientJobsConfiguration : public TaskManagerConfiguration
{
public:
   static std::string MsgNoJobList;
   static std::string MsgUnsupportedObjects;

   ClientJobsConfiguration(const int _debugOption = DebugOutput::UNDEFINED);

   virtual ClientWorkManager* BuildWorkList(const bool withProfiling) const;

   bool LoadFromConfigurationObject(ConfigurationObject* confObject,
                                    std::vector<std::string> &errorMessages);

private:
    virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::string> &errorMessages);
    virtual void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::string> &errorMessages);
    virtual bool IsConfigurationConsistent(std::vector<std::string> &errorMessages);

    virtual void SaveContentToOpenedFile(std::ofstream& file);

    ConfigurationObject* FindJobListObject(const std::list<ConfigurationObject*>& objectList);

    void FillJobList(ConfigurationObject* jobListObj, std::vector<std::string> &errorMessages);

    int debugOption;
};

#endif // CLIENTJOBSCONFIGURATION_H

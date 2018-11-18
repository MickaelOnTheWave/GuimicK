#ifndef CLIENTJOBSCONFIGURATION_H
#define CLIENTJOBSCONFIGURATION_H

#include <string>
#include <vector>

#include "abstracttypeconfiguration.h"
#include "clientworkmanager.h"
#include "configurationobject.h"

class ClientJobsConfiguration : public AbstractTypeConfiguration
{
public:
   static std::string MsgNoJobList;
   static std::string MsgUnsupportedObjects;

   ClientJobsConfiguration(const int _debugOption = DebugOutput::UNDEFINED);
   ClientJobsConfiguration(const ClientJobsConfiguration& other);

   virtual ConfigurationType GetType() const;

   virtual AbstractTypeConfiguration* Copy() const;

   bool LoadFromBuffer(const std::string& buffer, std::vector<std::string> &errorMessages);

   virtual void SaveToOpenedFile(std::ofstream& fileStream);

   virtual void GetJobList(std::list<AbstractJob*>& _jobList);

   virtual void SetJobList(const std::vector<AbstractJob*>& _jobList);

   virtual void ClearJobList();

   bool LoadFromConfigurationObject(ConfigurationObject* confObject,
                                    std::vector<std::string> &errorMessages);

private:
    virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::string> &errorMessages);
    virtual void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::string> &errorMessages);
    virtual bool IsConfigurationConsistent(std::vector<std::string> &errorMessages);

    ConfigurationObject* FindJobListObject(const std::list<ConfigurationObject*>& objectList);

    void FillJobList(ConfigurationObject* jobListObj, std::vector<std::string> &errorMessages);

    std::list<AbstractJob*> jobList;
    int debugOption;
};

#endif // CLIENTJOBSCONFIGURATION_H

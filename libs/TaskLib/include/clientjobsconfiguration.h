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
   static std::wstring MsgNoJobList;
   static std::wstring MsgUnsupportedObjects;

   ClientJobsConfiguration(const int _debugOption = DebugOutput::UNDEFINED);
   ClientJobsConfiguration(const ClientJobsConfiguration& other);

   virtual ConfigurationType GetType() const;

   virtual AbstractTypeConfiguration* Copy() const;

   bool LoadFromBuffer(const std::wstring& buffer, std::vector<std::wstring> &errorMessages);

   virtual void SaveToOpenedFile(std::wofstream& fileStream);

   virtual void GetJobList(std::list<AbstractJob*>& _jobList);

   virtual void SetJobList(const std::vector<AbstractJob*>& _jobList);

   virtual void ClearJobList();

   bool LoadFromConfigurationObject(ConfigurationObject* confObject,
                                    std::vector<std::wstring> &errorMessages);

private:
    virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                         std::vector<std::wstring> &errorMessages);
    virtual void FillGlobalProperties(ConfigurationObject* object,
                         std::vector<std::wstring> &errorMessages);
    virtual bool IsConfigurationConsistent(std::vector<std::wstring> &errorMessages);

    ConfigurationObject* FindJobListObject(const std::list<ConfigurationObject*>& objectList);

    void FillJobList(ConfigurationObject* jobListObj, std::vector<std::wstring> &errorMessages);

    std::list<AbstractJob*> jobList;
    int debugOption;
};

#endif // CLIENTJOBSCONFIGURATION_H

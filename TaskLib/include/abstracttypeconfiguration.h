#ifndef ABSTRACTTYPECONFIGURATION_H
#define ABSTRACTTYPECONFIGURATION_H

#include <string>
#include <vector>

#include "abstractjob.h"
#include "configurationparser.h"
#include "configurationtype.h"

class AbstractTypeConfiguration
{
public:
   AbstractTypeConfiguration();
   AbstractTypeConfiguration(const AbstractTypeConfiguration& other);
   virtual ~AbstractTypeConfiguration();

   virtual ConfigurationType GetType() const = 0;

   virtual AbstractTypeConfiguration* Copy() const = 0;

   virtual bool Load(ConfigurationParser& parser,
                     std::vector<std::wstring> &errorMessages);

   virtual void SaveToOpenedFile(std::wofstream& fileStream) = 0;

   virtual void GetJobList(std::list<AbstractJob*>& _jobList) = 0;

   virtual void SetJobList(const std::vector<AbstractJob*>& _jobList) = 0;

   virtual void ClearJobList() = 0;

protected:
   bool hasFatalError;

private:
    virtual void FillRootObjects(const std::list<ConfigurationObject*>& objectList,
                                 std::vector<std::wstring> &errorMessages) = 0;
    virtual void FillGlobalProperties(ConfigurationObject* object,
                                      std::vector<std::wstring> &errorMessages) = 0;
    virtual bool IsConfigurationConsistent(std::vector<std::wstring> &errorMessages) = 0;
};

#endif // ABSTRACTTYPECONFIGURATION_H

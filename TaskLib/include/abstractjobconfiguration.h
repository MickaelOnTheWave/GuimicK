#ifndef ABSTRACTJOBCONFIGURATION_H
#define ABSTRACTJOBCONFIGURATION_H

#include <string>

#include "abstractjob.h"
#include "configurationobject.h"

class AbstractJobConfiguration
{
public:
    AbstractJobConfiguration(const std::wstring& tag);
    virtual ~AbstractJobConfiguration();

    std::wstring GetTagName() const;

    AbstractJob* CreateConfiguredJob(ConfigurationObject* confObject,
                                     std::vector<std::wstring> &errorMessages);

    virtual bool IsRightJob(AbstractJob* job) = 0;
    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job) = 0;

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                            ConfigurationObject* confObject,
                            std::vector<std::wstring> &errorMessages);

    virtual void AnalyzeConfiguration(ConfigurationObject* confObject);
    virtual AbstractJob* CreateJob() = 0;
    virtual void ConfigureJob(AbstractJob* job, ConfigurationObject* confObject,
                              std::vector<std::wstring> &errorMessages);

    virtual void FillKnownProperties(std::vector<std::wstring>& properties);
    virtual void FillKnownSubObjects(std::vector<std::wstring>& objects);

    std::wstring jobTag;

private:
    void CheckKnownProperties(ConfigurationObject *confObject,
                              std::vector<std::wstring> &errorMessages);
    void CheckKnownSubObjects(ConfigurationObject *confObject,
                              std::vector<std::wstring> &errorMessages);
    void FillNumberedProperties(std::vector<std::wstring>& objects);

    bool HasValue(const std::vector<std::wstring>& collection,
                  const std::wstring& value) const;
    std::wstring BuildErrorMessage(const std::wstring& objectType,
                                  const std::wstring& objectName);
};

#endif // ABSTRACTJOBCONFIGURATION_H

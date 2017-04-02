#ifndef ABSTRACTJOBCONFIGURATION_H
#define ABSTRACTJOBCONFIGURATION_H

#include <string>

#include "abstractjob.h"
#include "configurationobject.h"

class AbstractJobConfiguration
{
public:
    AbstractJobConfiguration(const std::string& tag);
    virtual ~AbstractJobConfiguration();

    std::string GetTagName() const;

    AbstractJob* CreateConfiguredJob(ConfigurationObject* confObject,
                                     std::vector<std::string> &errorMessages);

protected:
    virtual AbstractJob* CreateConfiguredJobAfterCheck(
                            ConfigurationObject* confObject,
                            std::vector<std::string> &errorMessages) = 0;
    virtual void FillKnownProperties(std::vector<std::string>& properties);
    virtual void FillKnownSubObjects(std::vector<std::string>& objects);

private:
    void CheckKnownProperties(ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);
    void CheckKnownSubObjects(ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);

    bool HasValue(const std::vector<std::string>& collection,
                  const std::string& value) const;
    std::string BuildErrorMessage(const std::string& objectType,
                                  const std::string& objectName);

    std::string jobTag;

};

#endif // ABSTRACTJOBCONFIGURATION_H

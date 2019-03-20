#ifndef ABSTRACTBACKUPJOBCONFIGURATION_H
#define ABSTRACTBACKUPJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

#include "abstractbackupjob.h"

class AbstractBackupJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::wstring TargetProperty;
    static const std::wstring JoinReportsProperty;

    AbstractBackupJobConfiguration(const std::wstring& tag);

    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob* job);

    virtual std::wstring GetBackupItemName() const = 0;

protected:
    virtual void ConfigureJob(AbstractJob *job,
                              ConfigurationObject *confObject,
                              std::vector<std::wstring>& errorMessages);

    virtual void FillKnownProperties(std::vector<std::wstring>& properties);
    virtual void FillKnownSubObjects(std::vector<std::wstring>& objects);

private:
    void ConfigureItemList(AbstractBackupJob *job,
                           ConfigurationObject *confObject,
                           std::vector<std::wstring>& errorMessages);
    void ConfigureTarget(AbstractBackupJob *job,
                         ConfigurationObject *confObject);


};

#endif // ABSTRACTBACKUPJOBCONFIGURATION_H

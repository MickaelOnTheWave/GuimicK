#ifndef ABSTRACTBACKUPJOBCONFIGURATION_H
#define ABSTRACTBACKUPJOBCONFIGURATION_H

#include "abstractjobdefaultconfiguration.h"

#include "abstractbackupjob.h"

class AbstractBackupJobConfiguration : public AbstractJobDefaultConfiguration
{
public:
    static const std::string TargetProperty;
    static const std::string JoinReportsProperty;

    AbstractBackupJobConfiguration(const std::string& tag);

    virtual std::string GetBackupItemName() const = 0;

protected:
    virtual void ConfigureJob(AbstractJob *job,
                              ConfigurationObject *confObject,
                              std::vector<std::string>& errorMessages);

    virtual void FillKnownProperties(std::vector<std::string>& properties);
    virtual void FillKnownSubObjects(std::vector<std::string>& objects);

private:
    void ConfigureItemList(AbstractBackupJob *job,
                           ConfigurationObject *confObject,
                           std::vector<std::string>& errorMessages);
    void ConfigureTarget(AbstractBackupJob *job,
                         ConfigurationObject *confObject);


};

#endif // ABSTRACTBACKUPJOBCONFIGURATION_H

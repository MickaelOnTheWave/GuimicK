#ifndef GITBACKUPJOBCONFIGURATION_H
#define GITBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"

class GitBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    static const std::string WriteLogProperty;

    GitBackupJobConfiguration();

protected:
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);

private:
    virtual std::string GetBackupItemName() const;
};

#endif // GITBACKUPJOBCONFIGURATION_H

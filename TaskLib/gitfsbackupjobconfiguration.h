#ifndef GITFSBACKUPJOBCONFIGURATION_H
#define GITFSBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"

class GitFsBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    GitFsBackupJobConfiguration();

protected:
    virtual AbstractJob *CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::string>& properties);

private:
    virtual std::string GetBackupItemName() const;

};

#endif // GITFSBACKUPJOBCONFIGURATION_H

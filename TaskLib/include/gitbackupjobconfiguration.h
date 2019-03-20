#ifndef GITBACKUPJOBCONFIGURATION_H
#define GITBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"

class GitBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    static const std::wstring WriteLogProperty;

    GitBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob *job);

protected:
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::wstring> &errorMessages);
    virtual void FillKnownProperties(std::vector<std::wstring>& properties);

private:
    virtual std::wstring GetBackupItemName() const;
};

#endif // GITBACKUPJOBCONFIGURATION_H

#ifndef RSNAPSHOTBACKUPJOBCONFIGURATION_H
#define RSNAPSHOTBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"
#include "rsnapshotrawbackupjob.h"
#include "rsnapshotsmartbackupjob.h"

class RsnapshotBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    RsnapshotBackupJobConfiguration();

protected:

    virtual void AnalyzeConfiguration(ConfigurationObject* confObject);
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::string> &errorMessages);

    virtual void FillKnownProperties(std::vector<std::string>& properties);

private:
    virtual std::string GetBackupItemName() const;

    void ConfigureSmartJob(RsnapshotSmartBackupJob* job,
                           ConfigurationObject *confObject,
                           std::vector<std::string> &errorMessages);
    void ConfigureRawJob(RsnapshotRawBackupJob* job,
                         ConfigurationObject *confObject,
                         std::vector<std::string> &errorMessages);

    std::string GetRepositoryValue(ConfigurationObject *confObject) const;
    bool GetWaitAfterRunValue(ConfigurationObject *confObject) const;

    std::string fullConfigurationFile;
};

#endif // RSNAPSHOTBACKUPJOBCONFIGURATION_H

#ifndef RSNAPSHOTBACKUPJOBCONFIGURATION_H
#define RSNAPSHOTBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"
#include "rsnapshotrawbackupjob.h"
#include "rsnapshotsmartbackupjob.h"

class RsnapshotBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    static const std::string FullConfigurationProperty;
    static const std::string WaitProperty;
    static const std::string TemplateConfigurationProperty;
    static const std::string RepositoryProperty;
    static const std::string MaxBackupCountProperty;

    RsnapshotBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob* job);
    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob *job);

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
    bool GetWaitBeforeRunValue(ConfigurationObject *confObject) const;
    void SetMaxBackupCount(RsnapshotSmartBackupJob* job, ConfigurationObject* confObject) const;

    void CreateRawConfiguration(ConfigurationObject* conf, RsnapshotRawBackupJob* job);
    void CreateSmartConfiguration(ConfigurationObject* conf, RsnapshotSmartBackupJob* job);

    std::string fullConfigurationFile;
};

#endif // RSNAPSHOTBACKUPJOBCONFIGURATION_H

#ifndef RSNAPSHOTBACKUPJOBCONFIGURATION_H
#define RSNAPSHOTBACKUPJOBCONFIGURATION_H

#include "abstractbackupjobconfiguration.h"
#include "rsnapshotrawbackupjob.h"
#include "rsnapshotsmartbackupjob.h"

class RsnapshotBackupJobConfiguration : public AbstractBackupJobConfiguration
{
public:
    static const std::wstring FullConfigurationProperty;
    static const std::wstring WaitProperty;
    static const std::wstring TemplateConfigurationProperty;
    static const std::wstring RepositoryProperty;
    static const std::wstring MaxBackupCountProperty;

    RsnapshotBackupJobConfiguration();

    virtual bool IsRightJob(AbstractJob* job);
    virtual ConfigurationObject* CreateConfigurationObject(AbstractJob *job);

protected:
    virtual void AnalyzeConfiguration(ConfigurationObject* confObject);
    virtual AbstractJob* CreateJob();
    virtual void ConfigureJob(AbstractJob* job,
                              ConfigurationObject *confObject,
                              std::vector<std::wstring> &errorMessages);

    virtual void FillKnownProperties(std::vector<std::wstring>& properties);

private:
    virtual std::wstring GetBackupItemName() const;

    void ConfigureSmartJob(RsnapshotSmartBackupJob* job,
                           ConfigurationObject *confObject,
                           std::vector<std::wstring> &errorMessages);
    void ConfigureRawJob(RsnapshotRawBackupJob* job,
                         ConfigurationObject *confObject,
                         std::vector<std::wstring> &errorMessages);

    std::wstring GetRepositoryValue(ConfigurationObject *confObject) const;
    bool GetWaitBeforeRunValue(ConfigurationObject *confObject) const;
    void SetMaxBackupCount(RsnapshotSmartBackupJob* job, ConfigurationObject* confObject) const;

    void CreateRawConfiguration(ConfigurationObject* conf, RsnapshotRawBackupJob* job);
    void CreateSmartConfiguration(ConfigurationObject* conf, RsnapshotSmartBackupJob* job);

    std::wstring fullConfigurationFile;
};

#endif // RSNAPSHOTBACKUPJOBCONFIGURATION_H

#ifndef ABSTRACTBACKUPJOBCONFIGURATION_H
#define ABSTRACTBACKUPJOBCONFIGURATION_H

#include "abstractjobconfiguration.h"

#include "abstractbackupjob.h"

class AbstractBackupJobConfiguration : public AbstractJobConfiguration
{
public:
    AbstractBackupJobConfiguration(const std::string& tag);

protected:
    virtual void FillKnownProperties(std::vector<std::string>& properties);
    void ConfigureJob(AbstractBackupJob* job, ConfigurationObject *confObject);

};

#endif // ABSTRACTBACKUPJOBCONFIGURATION_H

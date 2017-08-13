#ifndef RSNAPSHOTCONFIGURATIONBUILDER_H
#define RSNAPSHOTCONFIGURATIONBUILDER_H

#include "abstractbackupjob.h"

class RsnapshotConfigurationBuilder
{
public:
    RsnapshotConfigurationBuilder(const std::string& templateConfigurationFile = "");

    void SetRepository(const std::string& value);
    void SetTemplateConfigurationFile(const std::string& file);
    void SetGeneratedConfigurationFile(const std::string& file);

    std::string CreateConfigurationFile(const AbstractBackupJob::BackupCollection& dataToBackup);

private:
    void BuildConfigurationFile(const AbstractBackupJob::BackupCollection &dataToBackup);
    std::string GetTemplateConfiguration(void) const;
    void AppendBackupData(std::string& configurationData,
                          const AbstractBackupJob::BackupCollection &dataToBackup) const;
    void CheckAndFixConfigurationFile(void);

    std::string templateFile;
    std::string configurationFile;
    std::string repository;
};

#endif // RSNAPSHOTCONFIGURATIONBUILDER_H

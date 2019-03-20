#ifndef RSNAPSHOTCONFIGURATIONBUILDER_H
#define RSNAPSHOTCONFIGURATIONBUILDER_H

#include "abstractbackupjob.h"

class RsnapshotConfigurationBuilder
{
public:
    RsnapshotConfigurationBuilder(const std::wstring& templateConfigurationFile = L"");

    void SetRepository(const std::wstring& value);
    void SetTemplateConfigurationFile(const std::wstring& file);
    void SetGeneratedConfigurationFile(const std::wstring& file);

    std::wstring CreateConfigurationFile(const AbstractBackupJob::BackupCollection& dataToBackup,
                                        const int maxBackups);

private:
    void BuildConfigurationFile(const AbstractBackupJob::BackupCollection &dataToBackup,
                                const int maxBackups);
    std::wstring GetTemplateConfiguration(void) const;

    void AppendMaxBackups(std::wstring& configurationData, const int maxBackups);
    void AppendBackupData(std::wstring& configurationData,
                          const AbstractBackupJob::BackupCollection &dataToBackup) const;
    void CheckAndFixConfigurationFile(void);

    std::wstring templateFile;
    std::wstring configurationFile;
    std::wstring repository;
};

#endif // RSNAPSHOTCONFIGURATIONBUILDER_H

#ifndef RSNAPSHOTSMARTCREATOR_H
#define RSNAPSHOTSMARTCREATOR_H

#include "rsnapshotbackupjob.h"

class RsnapshotSmartCreator
{
public:
    RsnapshotSmartCreator(const std::string& repositoryPath);

    void SetTemplateConfigurationFile(const std::string& file);
    void SetGeneratedConfigurationFile(const std::string& file);

    void AddFolderToBackup(const std::string& folder,
                           const std::string& destination);

    RsnapshotBackupJob* CreateConfiguredJob(void);

private:
    void BuildConfigurationFile(void);
    std::string GetTemplateConfiguration(void) const;
    void AppendBackupData(std::string& configurationData) const;
    void CheckAndFixConfigurationFile(void);

    std::vector<std::pair<std::string, std::string> > dataToBackup;
    std::string repository;
    std::string templateFile;
    std::string configurationFile;
};

#endif // RSNAPSHOTSMARTCREATOR_H

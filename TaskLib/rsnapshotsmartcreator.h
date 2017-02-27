#ifndef RSNAPSHOTSMARTCREATOR_H
#define RSNAPSHOTSMARTCREATOR_H

#include "rsnapshotbackupjob.h"

class RsnapshotSmartCreator
{
public:
    RsnapshotSmartCreator(const std::string& repositoryPath);

    void SetTemplateConfigurationFile(const std::string& file);

    void AddFolderToBackup(const std::string& folder,
                           const std::string& destination);

    RsnapshotBackupJob* CreateConfiguredJob(void) const;

private:
    void BuildConfigurationFile(void) const;
    std::string GetTemplateConfiguration(void) const;
    void AppendBackupData(std::string& configurationData) const;

    std::vector<std::pair<std::string, std::string> > dataToBackup;
    std::string repository;
    std::string templateFile;
};

#endif // RSNAPSHOTSMARTCREATOR_H

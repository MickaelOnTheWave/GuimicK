#include "rsnapshotbackupjobconfiguration.h"

#include "rsnapshotsmartcreator.h"

using namespace std;

RsnapshotBackupJobConfiguration::RsnapshotBackupJobConfiguration()
    : AbstractJobConfiguration("RsnapshotBackup")
{
}

AbstractJob *RsnapshotBackupJobConfiguration::CreateConfiguredJobAfterCheck(
                                                    ConfigurationObject *confObject,
                                                    std::vector<std::string> &)
{
    string repository = confObject->GetFirstProperty("repository", "param0");
    string configurationFile = confObject->GetFirstProperty("fullConfigurationFile", "param1");

    RsnapshotBackupJob* job = NULL;
    if (configurationFile != "")
        job = new RsnapshotBackupJob(repository, configurationFile);
    else
        job = CreateRsnapshotBackupJobFromCreator(confObject, repository);

    if (confObject->propertyList["showDebugInformation"] == "true")
        job->SetOutputDebugInformation(true);
    if (confObject->propertyList["waitAfterRun"] != "")
        job->SetWaitAfterRun(true);
    return job;
}

void RsnapshotBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    properties.push_back("repository");
    properties.push_back("fullConfigurationFile");
    properties.push_back("showDebugInformation");
    properties.push_back("waitAfterRun");
    properties.push_back("templateConfigurationFile");
}

void RsnapshotBackupJobConfiguration::FillKnownSubObjects(std::vector<std::string> &objects)
{
    objects.push_back("Folder");
}

RsnapshotBackupJob *RsnapshotBackupJobConfiguration::CreateRsnapshotBackupJobFromCreator(
                                                                ConfigurationObject *object,
                                                                const string &repository) const
{
    RsnapshotSmartCreator creator(repository);

    string templateFile = object->propertyList["templateConfigurationFile"];
    creator.SetTemplateConfigurationFile(templateFile);

    list<ConfigurationObject*>::iterator it = object->objectList.begin();
    for (; it != object->objectList.end(); it++)
    {
        ConfigurationObject* currentObj = *it;

        if (currentObj->name != "Folder")
            continue;

        string source(currentObj->propertyList["source"]);
        string dest(currentObj->propertyList["dest"]);
        creator.AddFolderToBackup(source, dest);
    }

    return creator.CreateConfiguredJob();
}

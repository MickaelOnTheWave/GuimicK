#include "gitfsbackupjobconfiguration.h"
#include "gitfsbackupjob.h"

using namespace std;

GitFsBackupJobConfiguration::GitFsBackupJobConfiguration()
    : AbstractJobConfiguration("GitFsBackup")
{
}

// TODO : refactor this and other backup jobs (see GitBackupJob) to remove
// code duplication
AbstractJob *GitFsBackupJobConfiguration::CreateConfiguredJobAfterCheck(
                                                ConfigurationObject *confObject,
                                                std::vector<std::string> &errorMessages)
{
    GitFsBackupJob* job = new GitFsBackupJob();

    list<ConfigurationObject*>::iterator it = confObject->objectList.begin();
    for (; it != confObject->objectList.end(); it++)
    {
        ConfigurationObject* currentObj = *it;

        if (currentObj->name == "Folder")
        {
            string source(currentObj->propertyList["source"]);
            string dest(currentObj->propertyList["dest"]);

            if (source != "" && dest != "")
                job->AddFolder(source, dest);
            else
                errorMessages.push_back("Error : invalid folder specified");
        }
    }

    string target(confObject->propertyList["target"]);
    if (target == "local")
        job->SetTargetLocal();
    else
        job->SetTargetRemote();

    return job;
}

void GitFsBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    properties.push_back("target");
}

void GitFsBackupJobConfiguration::FillKnownSubObjects(std::vector<std::string> &objects)
{
    objects.push_back("Folder");
}

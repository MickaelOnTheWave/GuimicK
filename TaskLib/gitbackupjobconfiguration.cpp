#include "gitbackupjobconfiguration.h"
#include "gitbackupjob.h"

using namespace std;

GitBackupJobConfiguration::GitBackupJobConfiguration()
    : AbstractJobConfiguration("GitBackup")
{
}

AbstractJob *GitBackupJobConfiguration::CreateConfiguredJobAfterCheck(
                                            ConfigurationObject *confObject,
                                            std::vector<std::string> &)
{
    GitBackupJob* job = new GitBackupJob();
    list<ConfigurationObject*>::iterator it = confObject->objectList.begin();
    for (; it != confObject->objectList.end(); it++)
    {
        ConfigurationObject* currentObj = *it;

        if (currentObj->name != "Repository")
            continue;

        string source(currentObj->propertyList["source"]);
        string dest(currentObj->propertyList["dest"]);
        job->AddFolder(source, dest);
    }

    string target(confObject->propertyList["target"]);
    if (target == "local")
        job->SetTargetLocal();
    else
        job->SetTargetRemote();

    string writeLogsToFiles(confObject->propertyList["writeLogsToFiles"]);
    if (writeLogsToFiles == "true")
        job->SetWriteLogsToFiles(true);

    string showDebugInformation(confObject->propertyList["showDebugInformation"]);
    if (showDebugInformation == "true")
        job->SetOutputDebugInformation(true);


    return job;
}

void GitBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    properties.push_back("target");
    properties.push_back("writeLogsToFiles");
}

void GitBackupJobConfiguration::FillKnownSubObjects(std::vector<std::string> &objects)
{
    objects.push_back("Repository");
}

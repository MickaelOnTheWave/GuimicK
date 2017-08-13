#include "abstractbackupjobconfiguration.h"

using namespace std;

static const string targetProperty = "target";
const string joinReportsProperty = "joinReports";

AbstractBackupJobConfiguration::AbstractBackupJobConfiguration(const std::string &tag)
    : AbstractJobDefaultConfiguration(tag)
{
}

void AbstractBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(targetProperty);
    properties.push_back(joinReportsProperty);
}

void AbstractBackupJobConfiguration::FillKnownSubObjects(std::vector<string> &objects)
{
    objects.push_back(GetBackupItemName());
}

void AbstractBackupJobConfiguration::ConfigureJob(AbstractJob *job,
                                                  ConfigurationObject *confObject,
                                                  std::vector<string> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    AbstractBackupJob* castJob = static_cast<AbstractBackupJob*>(job);

    ConfigureItemList(castJob, confObject, errorMessages);
    ConfigureTarget(castJob, confObject);
    castJob->SetJoinAllBackups(confObject->GetProperty(joinReportsProperty) != "false");
}

void AbstractBackupJobConfiguration::ConfigureItemList(AbstractBackupJob *job,
                                                       ConfigurationObject *confObject,
                                                       vector<string> &errorMessages)
{
    list<ConfigurationObject*>::const_iterator it = confObject->objectList.begin();
    for (; it != confObject->objectList.end(); ++it)
    {
        ConfigurationObject* currentObj = *it;

        if (currentObj->name == GetBackupItemName())
        {
            string source(currentObj->propertyList["source"]);
            string dest(currentObj->propertyList["dest"]);

            if (source != "" && dest != "")
                job->AddFolder(source, dest);
            else
                errorMessages.push_back("Error : invalid folder specified");
        }
    }
}

void AbstractBackupJobConfiguration::ConfigureTarget(AbstractBackupJob *job,
                                                     ConfigurationObject *confObject)
{
    string target(confObject->GetProperty(targetProperty));
    if (target == "local")
        job->SetTargetLocal();
    else
        job->SetTargetRemote();
}

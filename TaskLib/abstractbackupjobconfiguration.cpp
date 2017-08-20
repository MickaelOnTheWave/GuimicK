#include "abstractbackupjobconfiguration.h"

using namespace std;

const string AbstractBackupJobConfiguration::TargetProperty = "target";
const string AbstractBackupJobConfiguration::JoinReportsProperty = "joinReports";

static const string invalidSourceError = "Error : source is invalid";
static const string invalidDestinationError = "Error : destination is invalid";

AbstractBackupJobConfiguration::AbstractBackupJobConfiguration(const std::string &tag)
    : AbstractJobDefaultConfiguration(tag)
{
}

void AbstractBackupJobConfiguration::FillKnownProperties(std::vector<std::string> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);
    properties.push_back(TargetProperty);
    properties.push_back(JoinReportsProperty);
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
    castJob->SetJoinAllBackups(confObject->GetProperty(JoinReportsProperty) != "false");
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
            const string source(currentObj->GetProperty("source"));
            const string dest(currentObj->GetProperty("dest"));

            const bool isSourceValid = (source != "");
            const bool isDestinationValid = (dest != "");

            if (isSourceValid && isDestinationValid)
                job->AddFolder(source, dest);
            else if (isSourceValid == false)
                errorMessages.push_back(invalidSourceError);
            else // isDestinationValid == false
                errorMessages.push_back(invalidDestinationError);
        }
    }
}

void AbstractBackupJobConfiguration::ConfigureTarget(AbstractBackupJob *job,
                                                     ConfigurationObject *confObject)
{
    string target(confObject->GetProperty(TargetProperty));
    if (target == "local")
        job->SetTargetLocal();
    else
        job->SetTargetRemote();
}

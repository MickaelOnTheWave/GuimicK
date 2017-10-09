#include "taskmanagerconfiguration.h"

#include "configurationparser.h"
#include "profiledjob.h"

#include "changescreensaverjobconfiguration.h"
#include "clamavjobconfiguration.h"
#include "copyfsbackupjobconfigurations.h"
#include "diskspacecheckjobconfiguration.h"
#include "gitbackupjobconfiguration.h"
#include "gitfsbackupjobconfiguration.h"
#include "rsnapshotbackupjobconfiguration.h"
#include "shutdownjobconfiguration.h"
#include "sshconsolejobconfiguration.h"
#include "userconsolejobconfiguration.h"
#include "wakejobconfiguration.h"


using namespace std;

TaskManagerConfiguration::TaskManagerConfiguration()
    : client(NULL), hasFatalError(false)
{
   FillSupportedJobsList();
}

TaskManagerConfiguration::~TaskManagerConfiguration()
{
   delete client;
   FreeSupportedJobsList();
}

bool TaskManagerConfiguration::LoadFromFile(const string &fileName, vector<string> &errorMessages)
{
    errorMessages.clear();
    Reset();

    ConfigurationParser parser;
    bool result = parser.ParseFile(fileName, errorMessages);
    if (!result)
        return false;

    return SetupData(parser, errorMessages);
}

bool TaskManagerConfiguration::LoadFromBuffer(const string &content, vector<string> &errorMessages)
{
    errorMessages.clear();
    Reset();

    ConfigurationParser parser;
    bool result = parser.ParseBuffer(content, errorMessages);
    if (!result)
        return false;

    return SetupData(parser, errorMessages);
}

ClientWorkManager *TaskManagerConfiguration::BuildWorkList(const bool withProfiling) const
{
   return new ClientWorkManager(client->Clone(), withProfiling);
}

bool TaskManagerConfiguration::HasClient() const
{
    return (client != NULL);
}

Client* TaskManagerConfiguration::GetClient()
{
    return client;
}

string TaskManagerConfiguration::CreateWarning(const string &message) const
{
    return CreateMessage("Warning", message);
}

string TaskManagerConfiguration::CreateError(const string &message) const
{
    return CreateMessage("Error", message);
}

string TaskManagerConfiguration::CreateMessage(const string &tag, const string &message) const
{
    if (message != "")
        return tag + " : " + message;
    else
        return string("");
}

AbstractJob* TaskManagerConfiguration::CreateJobFromObject(ConfigurationObject* object,
                                                std::vector<string> &errorMessages)
{
    AbstractJobConfiguration* relatedConfiguration = GetJobConfiguration(object->name);
    if (relatedConfiguration == NULL)
    {
        string errorMessage = "unknown job \"";
        errorMessage += object->name + "\". Ignoring...";
        errorMessages.push_back(CreateWarning(errorMessage));
        return NULL;
    }
    else
        return relatedConfiguration->CreateConfiguredJob(object, errorMessages);
}

AbstractJobConfiguration *TaskManagerConfiguration::GetJobConfiguration(const string &jobTab)
{
    vector<AbstractJobConfiguration*>::iterator it = supportedJobs.begin();
    for (; it != supportedJobs.end(); ++it)
    {
        if ((*it)->GetTagName() == jobTab)
            return *it;
    }
    return NULL;
}

void TaskManagerConfiguration::FillSupportedJobsList()
{
    supportedJobs.push_back(new WakeJobConfiguration);
    supportedJobs.push_back(new ChangeScreensaverJobConfiguration);
    supportedJobs.push_back(new RsnapshotBackupJobConfiguration);
    supportedJobs.push_back(new ClamAvJobConfiguration);
    supportedJobs.push_back(new ShutdownJobConfiguration);
    supportedJobs.push_back(new UserConsoleJobConfiguration);
    supportedJobs.push_back(new SshConsoleJobConfiguration);
    supportedJobs.push_back(new GitBackupJobConfiguration);
    supportedJobs.push_back(new DiskSpaceCheckJobConfiguration);
    supportedJobs.push_back(new GitFsBackupJobConfiguration);
    supportedJobs.push_back(new RawCopyFsBackupJobConfiguration());
    supportedJobs.push_back(new RsyncCopyFsBackupJobConfiguration());
    supportedJobs.push_back(new ZipAndCopyFsBackupJobConfiguration());
}

void TaskManagerConfiguration::FreeSupportedJobsList()
{
   vector<AbstractJobConfiguration*>::iterator it=supportedJobs.begin();
   for (; it!=supportedJobs.end(); ++it)
       delete *it;
   supportedJobs.clear();
}

void TaskManagerConfiguration::Reset()
{
    jobList.clear();
    hasFatalError = false;
}

bool TaskManagerConfiguration::SetupData(const ConfigurationParser &parser,
                                         std::vector<string> &errorMessages)
{
    FillRootObjects(parser.objectList, errorMessages);
    FillGlobalProperties(parser.anonymousObject, errorMessages);
    return IsConfigurationConsistent(errorMessages);
}

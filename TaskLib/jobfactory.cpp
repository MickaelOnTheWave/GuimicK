#include "jobfactory.h"

#include "changescreensaverjobconfiguration.h"
#include "clamavjobconfiguration.h"
#include "copyfsbackupjobconfigurations.h"
#include "diskspacecheckjobconfiguration.h"
#include "gitbackupjobconfiguration.h"
#include "gitfsbackupjobconfiguration.h"
#include "remotejobsrunnerconfiguration.h"
#include "rsnapshotbackupjobconfiguration.h"
#include "shutdownjobconfiguration.h"
#include "sshconsolejobconfiguration.h"
#include "userconsolejobconfiguration.h"
#include "wakejobconfiguration.h"

using namespace std;

JobFactory::JobFactory()
{
   FillSupportedJobsList();
}

JobFactory::~JobFactory()
{
   FreeSupportedJobsList();
}

AbstractJob* JobFactory::CreateJob(ConfigurationObject* object,
                                  std::vector<string>& errorMessages)
{
   AbstractJobConfiguration* relatedConfiguration = GetConfiguration(object->name);
   if (relatedConfiguration == NULL)
   {
       string errorMessage = "unknown job \"";
       errorMessage += object->name + "\". Ignoring...";
       errorMessages.push_back(string("Warning : ") + errorMessage);
       return NULL;
   }
   else
      return relatedConfiguration->CreateConfiguredJob(object, errorMessages);
}

ConfigurationObject* JobFactory::CreateConfigurationObject(AbstractJob* job)
{
   AbstractJobConfiguration *jobConf = GetConfiguration(job);
   if (jobConf)
      return jobConf->CreateConfigurationObject(job);
   else
      return NULL;
}

AbstractJobConfiguration *JobFactory::GetConfiguration(const string &jobTab)
{
    vector<AbstractJobConfiguration*>::iterator it = supportedJobs.begin();
    for (; it != supportedJobs.end(); ++it)
    {
        if ((*it)->GetTagName() == jobTab)
            return *it;
    }
    return NULL;
}

AbstractJobConfiguration* JobFactory::GetConfiguration(AbstractJob* job)
{
   vector<AbstractJobConfiguration*>::iterator it = supportedJobs.begin();
   for (; it != supportedJobs.end(); ++it)
   {
      if ((*it)->IsRightJob(job))
         return *it;
   }
   return NULL;
}

void JobFactory::FillSupportedJobsList()
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
   supportedJobs.push_back(new RemoteJobsRunnerConfiguration());
}

void JobFactory::FreeSupportedJobsList()
{
   vector<AbstractJobConfiguration*>::iterator it=supportedJobs.begin();
   for (; it!=supportedJobs.end(); ++it)
       delete *it;
   supportedJobs.clear();
}

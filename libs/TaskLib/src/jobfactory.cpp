#include "jobfactory.h"

#include "clamavjobconfiguration.h"
#include "diskspacecheckjobconfiguration.h"
#include "gitbackupjobconfiguration.h"
#include "gitfsbackupjobconfiguration.h"
#include "oscopyfsbackupjobconfiguration.h"
#include "remotejobsrunnerconfiguration.h"
#include "rsnapshotbackupjobconfiguration.h"
#include "rsynccopyfsbackupjobconfiguration.h"
#include "shutdownjobconfiguration.h"
#include "sevenzipbackupjobconfiguration.h"
#include "sshconsolejobconfiguration.h"
#include "tarbackupjobconfiguration.h"
#include "tarincrementalbackupjobconfiguration.h"
#include "userconsolejobconfiguration.h"
#include "wakejobconfiguration.h"
#include "windowscheckdiskjobconfiguration.h"

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
                                  std::vector<wstring>& errorMessages)
{
   AbstractJobConfiguration* relatedConfiguration = GetConfiguration(object->GetName());
   if (relatedConfiguration == NULL)
   {
       wstring errorMessage = L"unknown job \"";
       errorMessage += object->GetName() + L"\". Ignoring...";
       errorMessages.push_back(wstring(L"Warning : ") + errorMessage);
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

AbstractJobConfiguration *JobFactory::GetConfiguration(const wstring &jobTab)
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
   supportedJobs.push_back(new RsnapshotBackupJobConfiguration);
   supportedJobs.push_back(new ClamAvJobConfiguration);
   supportedJobs.push_back(new ShutdownJobConfiguration);
   supportedJobs.push_back(new UserConsoleJobConfiguration);
   supportedJobs.push_back(new SshConsoleJobConfiguration);
   supportedJobs.push_back(new GitBackupJobConfiguration);
   supportedJobs.push_back(new DiskSpaceCheckJobConfiguration);
   supportedJobs.push_back(new GitFsBackupJobConfiguration);
   supportedJobs.push_back(new OsCopyFsBackupJobConfiguration());
   supportedJobs.push_back(new RsyncCopyFsBackupJobConfiguration());
   supportedJobs.push_back(new TarBackupJobConfiguration());
   supportedJobs.push_back(new SevenZipBackupJobConfiguration());
   supportedJobs.push_back(new RemoteJobsRunnerConfiguration());
   supportedJobs.push_back(new TarIncrementalBackupJobConfiguration());
   supportedJobs.push_back(new WindowsCheckDiskJobConfiguration());
}

void JobFactory::FreeSupportedJobsList()
{
   vector<AbstractJobConfiguration*>::iterator it=supportedJobs.begin();
   for (; it!=supportedJobs.end(); ++it)
       delete *it;
   supportedJobs.clear();
}

#include "sshconsolejobconfiguration.h"
#include "sshconsolejob.h"

#include "userconsolejob.h"
#include "userconsolejobconfiguration.h"

using namespace std;

SshConsoleJobConfiguration::SshConsoleJobConfiguration()
    : UserConsoleJobConfiguration("SshConsole")
{
}

SshConsoleJobConfiguration::~SshConsoleJobConfiguration()
{
}

bool SshConsoleJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<SshConsoleJob*>(job) != NULL);
}

ConfigurationObject* SshConsoleJobConfiguration::CreateConfigurationObject(AbstractJob* job)
{
   ConfigurationObject* confObject = UserConsoleJobConfiguration::CreateConfigurationObject(job);

   SshConsoleJob* sshJob = dynamic_cast<SshConsoleJob*>(job);
   if (sshJob)
   {
      UserConsoleJob* userJob = dynamic_cast<UserConsoleJob*>(sshJob->GetRemoteJob());
      if (userJob)
         ConfigureObjectFromJob(confObject, userJob);
   }

   return confObject;
}

AbstractJob *SshConsoleJobConfiguration::CreateConfiguredJobAfterCheck(
                                            ConfigurationObject *confObject,
                                            std::vector<std::string> &errorMessages)
{
    SshConsoleJob* mainJob = new SshConsoleJob("", "");
    AbstractJobDefaultConfiguration::ConfigureJob(mainJob, confObject, errorMessages);

    AbstractJob* remoteJob = UserConsoleJobConfiguration::CreateConfiguredJobAfterCheck(
                                                confObject, errorMessages);
    ConsoleJob* castRemote = static_cast<ConsoleJob*>(remoteJob);
    mainJob->SetRemoteJob(castRemote);
    return mainJob;
}

#include "remotejobsrunner.h"

#include "consolejob.h"
#include "clientjobsconfiguration.h"
#include "resultcollectionstatus.h"
#include "sshconsolejob.h"
#include "tools.h"

using namespace std;

string RemoteJobsRunner::TargetNotAccessibleError = "Client is offline";

RemoteJobsRunner::RemoteJobsRunner()
    : AbstractJob(),
      configurationFile(".taskmanager"),
      host(""), user(""), isWorkListTimed(true),
      originalClient(NULL)
{
}

RemoteJobsRunner::RemoteJobsRunner(const RemoteJobsRunner& other)
   : AbstractJob(other),
     configurationFile(other.configurationFile),
     host(other.host), user(other.user),
     isWorkListTimed(other.isWorkListTimed),
     originalClient(other.originalClient)

{
}

string RemoteJobsRunner::GetName()
{
   return string("RemoteJobsRunner");
}

AbstractJob* RemoteJobsRunner::Clone()
{
   return new RemoteJobsRunner(*this);
}

bool RemoteJobsRunner::InitializeFromClient(Client *client)
{
   bool ok = AbstractJob::InitializeFromClient(client);
   if (ok)
   {
      originalClient = client;
      host = client->GetProperty("ip");
      user = client->GetProperty("sshuser");
      return IsInitialized();
   }
   else
      return false;
}

bool RemoteJobsRunner::IsInitialized()
{
    return (host != "" && user != "");
}

JobStatus *RemoteJobsRunner::Run()
{
    if (!Tools::IsComputerAlive(host))
        return CreateErrorStatus(TargetNotAccessibleError);

    string content;
    bool ok = RetrieveRemoteConfiguration(content);
    if (!ok)
        return CreateErrorStatus(content);

    vector<string> configurationErrors;
    ClientJobsConfiguration configuration;
    bool usable = configuration.LoadFromBuffer(content, configurationErrors);
    if (!usable)
        return CreateConfigurationErrorStatus(configurationErrors);

    configuration.SetClient(originalClient->Clone());
    ClientWorkManager* workManager = configuration.BuildWorkList(isWorkListTimed);
    WorkResultData* remoteResults = workManager->RunWorkList();
    return new ResultCollectionStatus(remoteResults);
}

std::string RemoteJobsRunner::GetConfigurationFile() const
{
    return configurationFile;
}

void RemoteJobsRunner::SetConfigurationFile(const std::string &value)
{
    configurationFile = value;
}

bool RemoteJobsRunner::GetIsWorkListTimed() const
{
    return isWorkListTimed;
}

void RemoteJobsRunner::SetIsWorkListTimed(const bool value)
{
   isWorkListTimed = value;
}

bool RemoteJobsRunner::RetrieveRemoteConfiguration(string& output)
{
   ConsoleJob* retrieveJob = new ConsoleJob("cat", Tools::EscapedSpaces(configurationFile));
   SshConsoleJob remoteRetrieveJob(retrieveJob);
   remoteRetrieveJob.SetParentDebugManager(debugManager);
   remoteRetrieveJob.SetTarget(user, host);

   JobStatus* status = remoteRetrieveJob.Run();

   const bool success = (status->GetCode() == JobStatus::OK);
   if (success)
     output = remoteRetrieveJob.GetCommandOutput();
   else if (IsInvalidFileError())
     output = "Configuration file not found";
   else if (IsPasswordError())
     output = "Password needed";
   else
     output = "Error trying to retrieve configuration";

   delete status;
   return success;
}

JobStatus *RemoteJobsRunner::CreateErrorStatus(const string &message)
{
    return debugManager->CreateStatus(JobStatus::ERROR, message);
}

JobStatus *RemoteJobsRunner::CreateConfigurationErrorStatus(const std::vector<string>& errors)
{
    return CreateErrorStatus("Implement the real one");
}

bool RemoteJobsRunner::IsInvalidFileError() const
{
   // TODO : implement
   return false;
}

bool RemoteJobsRunner::IsPasswordError() const
{
   // TODO : implement
   return false;
}

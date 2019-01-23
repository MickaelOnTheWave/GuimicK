#include "remotejobsrunner.h"

#include "consolejob.h"
#include "clientjobsconfiguration.h"
#include "resultcollectionstatus.h"
#include "sshconsolejob.h"
#include "tools.h"

using namespace std;

string RemoteJobsRunner::TargetNotAccessibleError = "Client is offline";

RemoteJobsRunner::RemoteJobsRunner()
    : AbstractJob("RemoteJobsRunner"),
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

    list<AbstractJob*> clientJobs;
    configuration.GetJobList(clientJobs);
    ClientWorkManager* workManager = new ClientWorkManager(originalClient);
    workManager->AddJobs(clientJobs, isWorkListTimed);
    WorkResultData* remoteResults = workManager->RunWorkList();
    delete workManager;
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

   const bool success = (status->GetCode() == JobStatus::Ok);
   const string commandOutput = remoteRetrieveJob.GetCommandOutput();
   if (success)
     output = commandOutput;
   else if (IsInvalidFileError(commandOutput, configurationFile))
     output = "Configuration file not found";
   else if (IsPasswordError(commandOutput))
     output = "Password needed";
   else
     output = "Error trying to retrieve configuration";

   delete status;
   return success;
}

JobStatus *RemoteJobsRunner::CreateErrorStatus(const string &message)
{
    return debugManager->CreateStatus(JobStatus::Error, message);
}

JobStatus *RemoteJobsRunner::CreateConfigurationErrorStatus(const std::vector<string>& errors)
{
   JobStatus* status = new JobStatus(JobStatus::Error, "Errors in remote configuration");
   status->AddFileBuffer(GetAttachmentName(), CreateConfigurationErrorDescription(errors));
   return status;
}

bool RemoteJobsRunner::IsInvalidFileError(const string& output, const string& file) const
{
   size_t catCommandPos = output.find("cat:");
   const bool isCatCommand = (catCommandPos != string::npos);
   const bool isFileNotFound = (output.find(file + ": No such file or directory", catCommandPos));
   return isCatCommand && isFileNotFound;
}

bool RemoteJobsRunner::IsPasswordError(const string& output) const
{
   return (output.find("Permission denied") == 0);
}

string RemoteJobsRunner::CreateConfigurationErrorDescription(const std::vector<string>& errors) const
{
   string content;
   content += "The following errors occured while loading client configuration :\n";
   vector<string>::const_iterator it = errors.begin();
   for (; it!=errors.end(); ++it)
      content += string("\t") + *it + "\n";
   return content;
}

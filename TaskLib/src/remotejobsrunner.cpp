#include "remotejobsrunner.h"

#include "consolejob.h"
#include "clientjobsconfiguration.h"
#include "resultcollectionstatus.h"
#include "sshconsolejob.h"
#include "tools.h"

using namespace std;

wstring RemoteJobsRunner::TargetNotAccessibleError = L"Client is offline";

RemoteJobsRunner::RemoteJobsRunner()
    : AbstractJob(L"RemoteJobsRunner"),
      configurationFile(L".taskmanager"),
      host(L""), user(L""), isWorkListTimed(true),
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
      host = client->GetProperty(L"ip");
      user = client->GetProperty(L"sshuser");
      return IsInitialized();
   }
   else
      return false;
}

bool RemoteJobsRunner::IsInitialized()
{
    return (host != L"" && user != L"");
}

JobStatus *RemoteJobsRunner::Run()
{
    if (!Tools::IsComputerAlive(host))
        return CreateErrorStatus(TargetNotAccessibleError);

    wstring content;
    bool ok = RetrieveRemoteConfiguration(content);
    if (!ok)
        return CreateErrorStatus(content);

    vector<wstring> configurationErrors;
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

std::wstring RemoteJobsRunner::GetConfigurationFile() const
{
    return configurationFile;
}

void RemoteJobsRunner::SetConfigurationFile(const std::wstring &value)
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

bool RemoteJobsRunner::RetrieveRemoteConfiguration(wstring& output)
{
   ConsoleJob* retrieveJob = new ConsoleJob(L"cat", Tools::EscapedSpaces(configurationFile));
   SshConsoleJob remoteRetrieveJob(retrieveJob);
   remoteRetrieveJob.SetParentDebugManager(debugManager);
   remoteRetrieveJob.SetTarget(user, host);

   JobStatus* status = remoteRetrieveJob.Run();

   const bool success = (status->GetCode() == JobStatus::Ok);
   const wstring commandOutput = remoteRetrieveJob.GetCommandOutput();
   if (success)
     output = commandOutput;
   else if (IsInvalidFileError(commandOutput, configurationFile))
     output = L"Configuration file not found";
   else if (IsPasswordError(commandOutput))
     output = L"Password needed";
   else
     output = L"Error trying to retrieve configuration";

   delete status;
   return success;
}

JobStatus *RemoteJobsRunner::CreateErrorStatus(const wstring &message)
{
    return debugManager->CreateStatus(JobStatus::Error, message);
}

JobStatus *RemoteJobsRunner::CreateConfigurationErrorStatus(const std::vector<wstring>& errors)
{
   JobStatus* status = new JobStatus(JobStatus::Error, L"Errors in remote configuration");
   status->AddFileBuffer(GetAttachmentName(), CreateConfigurationErrorDescription(errors));
   return status;
}

bool RemoteJobsRunner::IsInvalidFileError(const wstring& output, const wstring& file) const
{
   size_t catCommandPos = output.find(L"cat:");
   const bool isCatCommand = (catCommandPos != wstring::npos);
   const bool isFileNotFound = (output.find(file + L": No such file or directory", catCommandPos));
   return isCatCommand && isFileNotFound;
}

bool RemoteJobsRunner::IsPasswordError(const wstring& output) const
{
   return (output.find(L"Permission denied") == 0);
}

wstring RemoteJobsRunner::CreateConfigurationErrorDescription(const std::vector<wstring>& errors) const
{
   wstring content;
   content += L"The following errors occured while loading client configuration :\n";
   vector<wstring>::const_iterator it = errors.begin();
   for (; it!=errors.end(); ++it)
      content += wstring(L"\t") + *it + L"\n";
   return content;
}

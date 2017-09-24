#include "remotejobsrunner.h"

#include "consolejob.h"
#include "sshconsolejob.h"
#include "tools.h"

using namespace std;

string RemoteJobsRunner::TargetNotAccessibleError = "Client is offline";

RemoteJobsRunner::RemoteJobsRunner()
    : configurationFile(".taskmanager"),
      host(""), user("")
{
}

bool RemoteJobsRunner::InitializeFromClient(Client *client)
{
    bool ok = AbstractJob::InitializeFromClient(client);
    if (ok)
    {
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

    JobStatus* status = new JobStatus();
    ClientWorkManager* workManager = CreateWorkManager(content, status);
    if (configErrors)
        return ConfigErrors;

    workList = CreateWorkList(workManager);
    workResults = RunWorkList(workList);
    return ResultCollectionStatus(workResults);
}

std::string RemoteJobsRunner::GetConfigurationFile() const
{
    return configurationFile;
}

void RemoteJobsRunner::SetConfigurationFile(const std::string &value)
{
    configurationFile = value;
}

bool RemoteJobsRunner::RetrieveRemoteConfiguration(string& output)
{
    SshConsoleJob remoteRetrieveJob(new ConsoleJob("ls", configurationFile));
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

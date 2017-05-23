#include "abstractbackupjob.h"

using namespace std;

AbstractBackupJob::AbstractBackupJob(const string &debugFileName)
    : repository(""), sshUser(""), sshHost(""),
      isTargetLocal(false), debugManager(debugFileName, false)
{
}

AbstractBackupJob::AbstractBackupJob(const AbstractBackupJob &other)
    : repository(other.repository),
      folderList(other.folderList),
      sshUser(other.sshUser), sshHost(other.sshHost),
      isTargetLocal(other.isTargetLocal),
      debugManager(other.debugManager)
{
}

bool AbstractBackupJob::InitializeFromClient(Client *client)
{
    if (isTargetLocal == false)
    {
        sshUser = client->GetProperty("sshuser");
        sshHost = client->GetProperty("ip");
    }

    return IsInitialized();
}

bool AbstractBackupJob::IsInitialized()
{
    return (isTargetLocal || IsRemoteTargetConsistent());
}

JobStatus *AbstractBackupJob::Run()
{
    vector<pair<JobStatus*, FileBackupReport*> > results;
    vector<pair<string, string> >::const_iterator it=folderList.begin();
    for (; it!=folderList.end(); it++)
        RunRepositoryBackup(it->first, it->second, results);

    return debugManager.UpdateStatus(CreateGlobalStatus(results));
}

void AbstractBackupJob::SetTargetRemote(const std::string &user, const std::string &host)
{
    isTargetLocal = false;
    sshHost = host;
    sshUser = user;
}

void AbstractBackupJob::SetTargetLocal()
{
    isTargetLocal = true;
    sshUser = "";
    sshHost = "";
}

void AbstractBackupJob::SetRepository(const string &value)
{
    repository = value;
}

void AbstractBackupJob::AddFolder(const std::string &source, const std::string &destination)
{
    folderList.push_back(make_pair(source, destination));
}

void AbstractBackupJob::GetFolderList(std::vector<std::pair<string, string> > &folders)
{
    copy(folderList.begin(), folderList.end(), back_inserter(folders));
}

void AbstractBackupJob::ClearFolderList()
{
    folderList.clear();
}

bool AbstractBackupJob::IsRemoteTargetConsistent() const
{
    return (sshUser != "" && sshHost != "");
}

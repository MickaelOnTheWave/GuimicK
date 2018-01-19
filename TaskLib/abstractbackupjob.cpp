#include "abstractbackupjob.h"

#include "backupstatusmanager.h"

using namespace std;

AbstractBackupJob::AbstractBackupJob(const string& _title)
    : AbstractJob(_title),
      repository(""), sshUser(""), sshHost(""), isTargetLocal(false)
{

    statusManager = new BackupStatusManager();
}

AbstractBackupJob::AbstractBackupJob(const AbstractBackupJob &other)
    : AbstractJob(other),
      repository(other.repository), folderList(other.folderList),
      sshUser(other.sshUser), sshHost(other.sshHost),
      isTargetLocal(other.isTargetLocal),
      statusManager(new BackupStatusManager(*other.statusManager))
{
}

AbstractBackupJob::~AbstractBackupJob()
{
    delete statusManager;
}

bool AbstractBackupJob::InitializeFromClient(Client *client)
{
    if (AbstractJob::InitializeFromClient(client))
    {
        if (Initialize() == false)
            return false;

        if (client && isTargetLocal == false)
        {
            sshUser = client->GetProperty("sshuser");
            sshHost = client->GetProperty("ip");
        }
        return IsInitialized();
    }
    else
        return false;
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

    return debugManager->UpdateStatus(CreateGlobalStatus(results));
}

JobStatus* AbstractBackupJob::RestoreBackup(const string& destination, const int repositoryIndex)
{
   if (repositoryIndex < folderList.size())
   {
      const string backupSource = folderList[repositoryIndex].second;
      return RestoreBackup(backupSource, destination);
   }
   else
      return new JobStatus(JobStatus::ERROR, "Invalid Repository Index");
}

bool AbstractBackupJob::IsTargetLocal() const
{
    return isTargetLocal;
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

void AbstractBackupJob::CopyTarget(const AbstractBackupJob& other)
{
   isTargetLocal = other.isTargetLocal;
   sshUser = other.sshUser;
   sshHost = other.sshHost;
}

string AbstractBackupJob::GetRepository() const
{
   return repository;
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

bool AbstractBackupJob::GetJoinReports() const
{
    return statusManager->GetJoinReports();
}

void AbstractBackupJob::SetJoinReports(const bool value)
{
    statusManager->SetJoinReports(value);
}

bool AbstractBackupJob::Initialize()
{
    statusManager->SetDebugManager(debugManager);
    statusManager->SetAttachmentName(GetAttachmentName());
    return true;
}

JobStatus *AbstractBackupJob::CreateGlobalStatus(const AbstractBackupJob::ResultCollection &results)
{
    return statusManager->CreateGlobalStatus(results, folderList);
}

bool AbstractBackupJob::IsRemoteTargetConsistent() const
{
    return (sshUser != "" && sshHost != "");
}

#include "abstractbackupjob.h"

#include "backupstatusmanager.h"

using namespace std;

AbstractBackupJob::AbstractBackupJob(const string& _title)
    : AbstractJob(_title),
      repository("")
{
    statusManager = new BackupStatusManager();
}

AbstractBackupJob::AbstractBackupJob(const AbstractBackupJob &other)
    : AbstractJob(other),
      repository(other.repository), folderList(other.folderList),
      target(other.target),
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

        if (client && target.isLocal == false)
        {
            target.sshUser = client->GetProperty("sshuser");
            target.sshHost = client->GetProperty("ip");
        }
        return IsInitialized();
    }
    else
        return false;
}

bool AbstractBackupJob::IsInitialized()
{
    return (target.isLocal || target.IsRemoteTargetConsistent());
}

JobStatus *AbstractBackupJob::Run()
{
    vector<pair<JobStatus*, FileBackupReport*> > results;
    vector<pair<string, string> >::const_iterator it=folderList.begin();
    for (; it!=folderList.end(); it++)
        RunRepositoryBackup(it->first, it->second, results);

    return debugManager->UpdateStatus(CreateGlobalStatus(results));
}

JobStatus* AbstractBackupJob::RestoreBackupFromServer(const string& destination, const int folderIndex)
{
   if (static_cast<unsigned int>(folderIndex) < folderList.size())
   {
      const string backupSource = folderList[folderIndex].second;
      return RestoreBackupFromServer(CreateBackupSourcePath(backupSource), destination);
   }
   else
      return new JobStatus(JobStatus::ERROR, "Invalid Repository Index");
}

JobStatus* AbstractBackupJob::RestoreBackupFromServer(const string& destination,
                                                      const int folderIndex,
                                                      const int)
{
   return RestoreBackupFromServer(destination, folderIndex, 0);
}

JobStatus* AbstractBackupJob::RestoreBackupFromClient(
      const BackupRestoreParameters& , const BackupRestoreTarget& )
{
   return new JobStatus(JobStatus::ERROR, "Not yet Implemented");
}

bool AbstractBackupJob::IsTargetLocal() const
{
    return target.isLocal;
}

void AbstractBackupJob::SetTargetRemote(const std::string &user, const std::string &host)
{
    target.isLocal = false;
    target.sshHost = host;
    target.sshUser = user;
}

void AbstractBackupJob::SetTargetLocal()
{
    target.isLocal = true;
    target.sshUser = "";
    target.sshHost = "";
}

void AbstractBackupJob::CopyTarget(const AbstractBackupJob& other)
{
   target = other.target;
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

int AbstractBackupJob::GetFolderCount() const
{
   return static_cast<int>(folderList.size());
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

string AbstractBackupJob::CreateBackupSourcePath(const string& backupTag) const
{
   return repository + backupTag;
}

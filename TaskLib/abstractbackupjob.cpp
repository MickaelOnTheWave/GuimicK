#include "abstractbackupjob.h"

#include "backupstatusmanager.h"

using namespace std;

AbstractBackupJob::AbstractBackupJob()
    : repository(""), sshUser(""), sshHost(""),
      isTargetLocal(false)
{
    debugManager = new JobDebugInformationManager(false);
    statusManager = new BackupStatusManager();
}

AbstractBackupJob::AbstractBackupJob(const AbstractBackupJob &other)
    : repository(other.repository),
      folderList(other.folderList),
      sshUser(other.sshUser), sshHost(other.sshHost),
      isTargetLocal(other.isTargetLocal),
      debugManager(new JobDebugInformationManager(*other.debugManager)),
      statusManager(new BackupStatusManager(*other.statusManager))
{
}

AbstractBackupJob::~AbstractBackupJob()
{
    if (isDebugManagerParent == false)
        delete debugManager;
    delete statusManager;
}

bool AbstractBackupJob::InitializeFromClient(Client *client)
{
    bool ok = Initialize();
    if (!ok)
        return false;

    if (client && isTargetLocal == false)
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

    return debugManager->UpdateStatus(CreateGlobalStatus(results));
}

void AbstractBackupJob::SetOutputDebugInformation(const int value)
{
    debugManager->SetUse(value);
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

void AbstractBackupJob::SetParentDebugManager(JobDebugInformationManager *manager)
{
    isDebugManagerParent = true;
    delete debugManager;
    debugManager = manager;
}

bool AbstractBackupJob::Initialize()
{
    debugManager->SetJobName(GetName());
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

#include "gitbackupjob.h"

#include <sstream>
#include <filetools.h>
#include <unistd.h>

using namespace std;

GitBackupJob::GitBackupJob(const std::vector<std::pair<string, string> > &_gitRepositoryList)
 : sshUser(""), sshHost(""), isTargetLocal(true)
{
    gitRepositoryList = _gitRepositoryList;
}

GitBackupJob::~GitBackupJob()
{
}

string GitBackupJob::GetName()
{
    return "Git Backup";
}

bool GitBackupJob::InitializeFromClient(Client *client)
{
    return (isTargetLocal) ? true : InitializeRemoteTarget(client);
}

bool GitBackupJob::IsInitialized()
{
    return (isTargetLocal) ? AreSourcesConsistent() : IsRemoteTargetConsistent();
}

JobStatus *GitBackupJob::Run()
{

}

void GitBackupJob::SetTargetRemote(const string &user, const string &host)
{
    sshUser = user;
    sshHost = host;
    isTargetLocal = false;
}

void GitBackupJob::SetTargetLocal()
{
    sshUser = "";
    sshHost = "";
    isTargetLocal = true;
}

void GitBackupJob::AddRepository(const string &sourcePath, const string &destPath)
{
    gitRepositoryList.push_back(make_pair<string, string>(sourcePath, destPath));
}

void GitBackupJob::ClearRepositoryList()
{
    gitRepositoryList.clear();
}

string GitBackupJob::CorrectRepositoryWord(int n)
{
    return (n == 1) ? "repository" : "repositories";
}

bool GitBackupJob::InitializeRemoteTarget(Client *client)
{
    if (sshUser == "")
        sshUser = client->GetProperty("sshuser");
    if (sshHost == "")
        sshHost = client->GetProperty("ip");
    return IsRemoteTargetConsistent();
}

bool GitBackupJob::IsRemoteTargetConsistent()
{
    return (sshUser != "" && sshHost != "");
}

bool GitBackupJob::AreSourcesConsistent() const
{
    vector<pair<string, string> >::const_iterator it=gitRepositoryList.begin();
    for (; it!=gitRepositoryList.end(); it++)
    {
        if (!FileTools::FolderExists(it->first))
            return false;
    }
    return true;
}

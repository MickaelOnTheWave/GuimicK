#include "gitclonejob.h"
#include <sstream>
#include <filetools.h>

using namespace std;

GitCloneJob::GitCloneJob(std::vector<std::pair<std::string, std::string> > *_gitRepositoryList,
                         std::string _sshUser, std::string _sshHost)
    : sshUser(_sshUser), sshHost(_sshHost)
{
    localTarget = (sshUser == "" || sshHost == "");
    if (_gitRepositoryList)
        gitRepositoryList = *_gitRepositoryList;
}

GitCloneJob::~GitCloneJob()
{

}

std::string GitCloneJob::GetName()
{
    return "Git Backup (clone)";
}

bool GitCloneJob::InitializeFromClient(Client *client)
{
    if (localTarget)
        return true;

    if (sshUser == "")
        sshUser = client->GetProperty("sshuser");

    if (sshHost == "")
        sshHost = client->GetProperty("ip");

    return (sshUser != "" && sshHost != "");
}

bool GitCloneJob::IsInitialized()
{
    bool consistentTarget = (localTarget) ? true : (sshUser != "" && sshHost != "");
    return consistentTarget && AreSourcesConsistent();
}

JobStatus *GitCloneJob::Run()
{  
    unsigned int faultyRepositories = 0;
    JobStatus* globalStatus = new JobStatus(JobStatus::OK);
    vector<pair<string, string> >::const_iterator it=gitRepositoryList.begin();
    vector<pair<string, string> >::const_iterator end=gitRepositoryList.end();
    for (; it!=end; it++)
    {
        string destination(it->second);
        if (FileTools::FolderExists(destination))
            FileTools::RemoveFolder(destination, false);

        ConsoleJob* gitCommand = new ConsoleJob("", "git", BuildGitParameters(it->first, destination));
        gitCommand->SetOutputToBuffer();
        JobStatus* repositoryStatus = gitCommand->Run();
        delete repositoryStatus;
        if (repositoryStatus->GetCode() == JobStatus::ERROR)
            ++faultyRepositories;

        delete gitCommand;
    }

    stringstream descriptionStream;
    if (faultyRepositories == 0)
    {
        globalStatus->SetCode(JobStatus::OK);
        descriptionStream << gitRepositoryList.size();
        descriptionStream << " Git " << CorrectRepositoryWord(gitRepositoryList.size()) << " successfully backed up.";
    }
    else if (faultyRepositories == gitRepositoryList.size())
    {
        globalStatus->SetCode(JobStatus::ERROR);
        descriptionStream << gitRepositoryList.size();
        descriptionStream << " Git " << CorrectRepositoryWord(gitRepositoryList.size()) << " failed to backup.";
    }
    else
    {
        globalStatus->SetCode(JobStatus::OK_WITH_WARNINGS);
        int okRepositories = gitRepositoryList.size()-faultyRepositories;
        descriptionStream << okRepositories;
        descriptionStream << " Git " << CorrectRepositoryWord(gitRepositoryList.size()) << " backed up, ";
        descriptionStream << faultyRepositories << " failed.";
    }

    globalStatus->SetDescription(descriptionStream.str());
    return globalStatus;
}

void GitCloneJob::SetRemoteTarget(const string &user, const string &host)
{
    sshUser = user;
    sshHost = host;
    localTarget = false;
}

void GitCloneJob::SetLocalTarget()
{
    sshUser = "";
    sshHost = "";
    localTarget = true;
}

void GitCloneJob::AddRepository(const std::string &sourcePath, const std::string &destPath)
{
    gitRepositoryList.push_back(make_pair<string, string>(sourcePath, destPath));
}

void GitCloneJob::ClearRepositoryList()
{
    gitRepositoryList.clear();
}

bool GitCloneJob::AreSourcesConsistent() const
{
    if (!localTarget)
        return true;

    // Check that all repositories are valid directories
    vector<pair<string, string> >::const_iterator it=gitRepositoryList.begin();
    vector<pair<string, string> >::const_iterator end=gitRepositoryList.end();
    for (; it!=end; it++)
    {
        if (!FileTools::FolderExists(it->first))
            return false;
    }

    return true;
}

string GitCloneJob::BuildGitParameters(const std::string& source, const std::string& destination) const
{
    string params("clone --mirror ");
    if (localTarget)
        params += source;
    else
        params += sshUser + "@" + sshHost + ":" + source;

    params += string(" ") + destination;

    return params;
}

string GitCloneJob::CorrectRepositoryWord(int n)
{
    return (n == 1) ? "repository" : "repositories";
}

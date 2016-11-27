#include "gitbackupjob.h"

#include <sstream>
#include <filetools.h>
#include <unistd.h>

#include "consolejob.h"
#include "gitreportparser.h"

using namespace std;

static const string invalidSourceRepositoryError        = "Invalid source repository";
static const string repositoryCloneOk                   = "Repository cloned successfully";
static const string invalidDestinationRepositoryError   = "Invalid destination repository";
static const string repositoryPullOk                    = "Repository successfully updated, see attached file.";


GitBackupJob::GitBackupJob()
 : sshUser(""), sshHost(""), isTargetLocal(true)
{
}

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
    vector<JobStatus*> repositoriesStatus;
    vector<pair<string, string> >::const_iterator it=gitRepositoryList.begin();
    for (; it!=gitRepositoryList.end(); it++)
        RunGitRepositoryBackup(it->first, it->second, repositoriesStatus);

    return CreateGlobalStatus(repositoriesStatus);
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

string GitBackupJob::GetCorrectRepositoryWord() const
{
    return (gitRepositoryList.size() == 1) ? "repository" : "repositories";
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

void GitBackupJob::RunGitRepositoryBackup(const string &source,
                                          const string &destination,
                                          vector<JobStatus*>& statusList) const
{
    if (FileTools::FolderExists(destination))
        RunGitPull(destination, statusList);
    else
        RunGitClone(source, destination, statusList);
}

void GitBackupJob::RunGitPull(const string &repository, std::vector<JobStatus *> &statusList) const
{
    char *originalDirectory = NULL;
    int originalDirectorySize = 0;
    originalDirectory = getcwd(originalDirectory, originalDirectorySize);
    chdir(repository.c_str());

    ConsoleJob* gitCommand = new ConsoleJob("", "git", "pull");
    gitCommand->SetOutputToBuffer();
    JobStatus* status = gitCommand->Run();

    if (gitCommand->GetCommandReturnCode() == 128)
        status->SetDescription(invalidDestinationRepositoryError);
    else
    {
        const string gitLogFile = FileTools::GetFilenameFromUnixPath(repository) + ".txt";
        GitReportParser parser;
        bool ok = parser.ParseBuffer(gitCommand->GetCommandOutput());
        if (ok)
        {
            status->AddFileBuffer(gitLogFile, parser.GetFullDescription());
            status->SetDescription(parser.GetMiniDescription());
        }
        else
        {
            status->AddFileBuffer(gitLogFile, gitCommand->GetCommandOutput());
            status->SetDescription(repositoryPullOk);
        }
    }

    statusList.push_back(status);
    delete gitCommand;

    chdir(originalDirectory);
}

void GitBackupJob::RunGitClone(const string &source,
                               const string &destination,
                               vector<JobStatus *> &statusList) const
{
    ConsoleJob* gitCommand = new ConsoleJob("", "git", BuildGitParameters(source, destination));
    const string gitLogFile = FileTools::GetFilenameFromUnixPath(destination) + ".txt";
    gitCommand->SetOutputToBuffer();
    JobStatus* status = gitCommand->Run();
    if (gitCommand->GetCommandReturnCode() == 128)
        status->SetDescription(invalidSourceRepositoryError);
    else
    {
        status->SetDescription(repositoryCloneOk);
        status->AddFileBuffer(gitLogFile, gitCommand->GetCommandOutput());
    }

    statusList.push_back(status);
    delete gitCommand;
}

string GitBackupJob::BuildGitParameters(const string &source, const string &destination) const
{
    string params("clone ");
    if (isTargetLocal)
        params += source;
    else
        params += sshUser + "@" + sshHost + ":" + source;

    params += string(" 2>&1 ") + destination;
    return params;
}

JobStatus *GitBackupJob::CreateGlobalStatus(const std::vector<JobStatus *> &statusList) const
{
    if (gitRepositoryList.size() == 1)
        return CreateSingleRepositoryStatus(statusList.front());
    else
        return CreateMultiRepositoryStatus(statusList);
}

JobStatus *GitBackupJob::CreateSingleRepositoryStatus(JobStatus *status) const
{
    return new JobStatus(*status);
}

JobStatus *GitBackupJob::CreateMultiRepositoryStatus(const std::vector<JobStatus *> &statusList) const
{
    JobStatus* status = new JobStatus();
    stringstream descriptionStream;
    unsigned int faultyRepositories = CountFaultyRepositories(statusList);
    if (faultyRepositories == 0)
    {
        status->SetCode(JobStatus::OK);
        descriptionStream << gitRepositoryList.size();
        descriptionStream << " Git " << GetCorrectRepositoryWord() << " successfully backed up.";
    }
    else if (faultyRepositories == gitRepositoryList.size())
    {
        status->SetCode(JobStatus::ERROR);
        descriptionStream << gitRepositoryList.size();
        descriptionStream << " Git " << GetCorrectRepositoryWord() << " failed to backup.";
    }
    else
    {
        status->SetCode(JobStatus::OK_WITH_WARNINGS);
        int okRepositories = gitRepositoryList.size()-faultyRepositories;
        descriptionStream << okRepositories;
        descriptionStream << " Git " << GetCorrectRepositoryWord() << " backed up, ";
        descriptionStream << faultyRepositories << " failed.";
    }

    std::vector<JobStatus *>::const_iterator it=statusList.begin();
    for (; it!=statusList.end(); ++it)
        status->AddAllFilesFromStatus(*it);

    status->SetDescription(descriptionStream.str());
    return status;
}

unsigned int GitBackupJob::CountFaultyRepositories(const vector<JobStatus *> &statusList) const
{
    unsigned int faulties = 0;
    vector<JobStatus*>::const_iterator it=statusList.begin();
    for(; it!=statusList.end(); ++it)
    {
        if ((*it)->GetCode() == JobStatus::ERROR)
            ++faulties;
    }
    return faulties;
}

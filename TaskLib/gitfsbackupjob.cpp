#include "gitfsbackupjob.h"

#include <unistd.h>
#include "filetools.h"
#include "tools.h"

using namespace std;

const string errorCreatingRepository = "Backup repository was not created";
const string errorCleaning = "Failed cleaning old data";
const string errorCopyingData = "Data was not properly copied";
const string errorAddingData = "Git add failed";
const string errorCommitingData = "Git commit failed";

// TODO : remove duplication from GitBackupJob
GitFsBackupJob::GitFsBackupJob()
{
}

GitFsBackupJob::GitFsBackupJob(const GitFsBackupJob &other)
    : repositoryList(other.repositoryList)
{
}

std::string GitFsBackupJob::GetName()
{
    return string("Git Filesystem Backup");
}

AbstractJob *GitFsBackupJob::Clone()
{
    return new GitFsBackupJob(*this);
}

bool GitFsBackupJob::InitializeFromClient(Client *)
{
    return IsInitialized();
}

bool GitFsBackupJob::IsInitialized()
{
    return false;
}

JobStatus *GitFsBackupJob::Run()
{
    vector<JobStatus*> statuses;
    vector<pair<string, string> >::const_iterator it=repositoryList.begin();
    for (; it!=repositoryList.end(); it++)
    {
        JobStatus* repositoryStatus = RunRepositoryBackup(it->first, it->second);
        statuses.push_back(repositoryStatus);
    }

    return CreateGlobalStatus(statuses);
}

void GitFsBackupJob::AddFolder(const string &folder, const string &repository)
{
    repositoryList.push_back(make_pair(folder, repository));
}

JobStatus* GitFsBackupJob::RunRepositoryBackup( const string &source,
                                                const string &destination)
{
    JobStatus* status = new JobStatus(JobStatus::OK);
    if (FileTools::FolderExists(destination) == false)
        CreateGitRepository(destination, status);

    if (status->GetCode() == JobStatus::OK)
        CleanDestination(destination, status);

    if (status->GetCode() == JobStatus::OK)
        CopyData(source, destination, status);

    if (status->GetCode() == JobStatus::OK)
        AddData(destination, status);

    if (status->GetCode() == JobStatus::OK)
        CommitData(destination, status);

    return status;
}

JobStatus *GitFsBackupJob::CreateGlobalStatus(std::vector<JobStatus *> &statuses)
{
    if (statuses.size() == 1)
        return new JobStatus(*statuses.front());
    else
        return new JobStatus(JobStatus::OK_WITH_WARNINGS, "Multiple backups. Create proper message.");
}

void GitFsBackupJob::CreateGitRepository(const string &path, JobStatus *status)
{
    string unusedOutput;
    const string command = string("git init ") + path;
    int returnValue = Tools::RunExternalCommandToBuffer(command, unusedOutput, true);
    if (returnValue == 0)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorCreatingRepository);
    }
}

void GitFsBackupJob::CleanDestination(const string &destination, JobStatus *status)
{
    string unusedOutput;
    const string command = string("rm -Rf ") + destination + "*";
    int returnValue = Tools::RunExternalCommandToBuffer(command, unusedOutput, true);
    if (returnValue == 0)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorCleaning);
    }
}

void GitFsBackupJob::CopyData(const string &source, const string &destination,
                              JobStatus *status)
{
    string unusedOutput;
    const string command = string("cp -R ") + source + "* " + destination;
    int returnValue = Tools::RunExternalCommandToBuffer(command, unusedOutput, true);
    if (returnValue == 0)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorCopyingData);
    }
}

void GitFsBackupJob::AddData(const string &destination, JobStatus *status)
{
    string originalDirectory = FileTools::GetCurrentFullPath();
    chdir(destination.c_str());

    string unusedOutput;
    int returnValue = Tools::RunExternalCommandToBuffer("git add -A", unusedOutput, true);
    if (returnValue == 0)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorAddingData);
    }

    chdir(originalDirectory.c_str());
}

void GitFsBackupJob::CommitData(const string &destination, JobStatus *status)
{
    string originalDirectory = FileTools::GetCurrentFullPath();
    chdir(destination.c_str());

    string unusedOutput;
    string command = string("git commit -m \"Automated backup\"");
    int returnValue = Tools::RunExternalCommandToBuffer(command, unusedOutput, true);
    if (returnValue == 0)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorCommitingData);
    }

    chdir(originalDirectory.c_str());
}

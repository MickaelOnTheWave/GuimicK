#include "gitfsbackupjob.h"

#include <unistd.h>

#include "filetools.h"
#include "gitcommitreportparser.h"
#include "tools.h"

using namespace std;

const string errorCreatingRepository = "Backup repository was not created";
const string errorCleaning = "Failed cleaning old data";
const string errorCopyingData = "Data was not properly copied";
const string errorAddingData = "Git add failed";
const string errorCommitingData = "Git commit failed";
const int emptyDirError = 256;

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

    string originalDirectory = FileTools::GetCurrentFullPath();
    chdir(destination.c_str());

    if (status->GetCode() == JobStatus::OK)
        AddData(status);

    string commitId("");
    if (status->GetCode() == JobStatus::OK)
        commitId = CommitData(status);

    if (status->GetCode() == JobStatus::OK)
        CreateReport(commitId, status);

    chdir(originalDirectory.c_str());

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
    if (returnValue == 0 || returnValue == emptyDirError)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorCopyingData);
    }
}

void GitFsBackupJob::AddData(JobStatus *status)
{
    string unusedOutput;
    int returnValue = Tools::RunExternalCommandToBuffer("git add -A", unusedOutput, true);
    if (returnValue == 0)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorAddingData);
    }
}

string GitFsBackupJob::CommitData(JobStatus *status)
{
    string commandOutput;
    string command = string("git commit -m \"Automated backup\"");
    int returnValue = Tools::RunExternalCommandToBuffer(command, commandOutput, true);
    if (returnValue == 0)
        return GetCommitId(commandOutput);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorCommitingData);
        return string("");
    }
}

void GitFsBackupJob::CreateReport(const string& commitId, JobStatus *status)
{
    string commandOutput;
    string command = string("git diff-tree --no-commit-id --name-status -r ");
    command += commitId;
    int returnValue = Tools::RunExternalCommandToBuffer(command, commandOutput, true);
    if (returnValue == 0)
    {
        GitCommitReportParser parser;
        bool ok = parser.ParseBuffer(commandOutput);
        if (ok)
        {
            status->SetCode(JobStatus::OK);
            status->SetDescription(parser.GetMiniDescription());
            status->AddFileBuffer("FsBackup.txt", parser.GetFullDescription());
            return;
        }
    }

    status->SetCode(JobStatus::OK_WITH_WARNINGS);
    status->SetDescription("Parsing failed");
}

string GitFsBackupJob::GetCommitId(const string &output)
{
    string commitString = output.substr(0, output.find_first_of(']'));
    size_t posStartId = commitString.find_last_of(' ');
    return commitString.substr(posStartId+1, commitString.size()-1);
}

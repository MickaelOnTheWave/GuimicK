#include "gitfsbackupjob.h"

#include <stdlib.h>
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

GitFsBackupJob::GitFsBackupJob()
    : AbstractBackupJob()
{
}

GitFsBackupJob::GitFsBackupJob(const GitFsBackupJob &other)
    : AbstractBackupJob(other)
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

JobStatus *GitFsBackupJob::Run()
{
    vector<JobStatus*> statuses;
    vector<pair<string, string> >::const_iterator it=folderList.begin();
    for (; it!=folderList.end(); it++)
    {
        JobStatus* repositoryStatus = RunRepositoryBackup(it->first, it->second);
        statuses.push_back(repositoryStatus);
    }

    return CreateGlobalStatus(statuses);
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
    int returnValue = RunCopyCommand(source, destination);
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
    int revisionCount = GetRevisionCount();
    if (revisionCount < 2)
        CreateInitialReport(status);
    else
        CreateDifferentialReport(commitId, status);
}

int GitFsBackupJob::RunCopyCommand(const string &source, const string &destination)
{
    string unusedOutput;
    string command;
    if (isTargetLocal)
        command = "cp -R ";
    else
        command = string("scp -r ") + sshUser + "@" + sshHost + ":";
    command += source + "* " + destination;

    return Tools::RunExternalCommandToBuffer(command, unusedOutput, true);
}

int GitFsBackupJob::GetRevisionCount() const
{
    string output;
    const string command = "git rev-list --all --count";
    int returnValue = Tools::RunExternalCommandToBuffer(command, output, true);
    return (returnValue == 0) ? atoi(output.c_str()) : -1;
}

void GitFsBackupJob::CreateInitialReport(JobStatus *status)
{
    string output;
    int returnValue = Tools::RunExternalCommandToBuffer("ls", output, true);
    if (returnValue != 0)
    {
        status->SetCode(JobStatus::OK_WITH_WARNINGS);
        status->SetDescription("Report creation failed");
        status->AddFileBuffer("GitFsBackup.txt", output);
    }
    else
    {
        vector<string> fileList;
        Tools::TokenizeString(output, '\n', fileList);
        FileBackupReport report;

        vector<string>::const_iterator it = fileList.begin();
        for(; it!=fileList.end(); ++it)
            report.AddAsAdded(*it);

        status->SetCode(JobStatus::OK);
        status->SetDescription(report.GetMiniDescription());
        status->AddFileBuffer("FsBackup.txt", report.GetFullDescription());
    }
}

void GitFsBackupJob::CreateDifferentialReport(const string &commitId, JobStatus *status)
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

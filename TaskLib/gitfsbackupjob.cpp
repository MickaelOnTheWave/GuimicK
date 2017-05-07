#include "gitfsbackupjob.h"

#include <stdlib.h>
#include <unistd.h>

#include "consolejob.h"
#include "filetools.h"
#include "gitcommitreportparser.h"
#include "tools.h"

using namespace std;

const string errorCreatingRepository = "Backup repository was not created";
const string errorCleaning = "Failed cleaning old data";
const string errorCopyingData = "Data was not properly copied";
const string errorAddingData = "Git add failed";
const string errorCommitingData = "Git commit failed";
const string errorGitNotInstalled = "Git not installed";
const int emptyDirError = 1;

GitFsBackupJob::GitFsBackupJob()
    : AbstractBackupJob(), debugManager("GitFsBackup", false),
      usingDebugInformation(false),
      joinRepositoriesReports(true)
{
}

GitFsBackupJob::GitFsBackupJob(const GitFsBackupJob &other)
    : AbstractBackupJob(other), debugManager(other.debugManager),
      usingDebugInformation(other.usingDebugInformation),
      joinRepositoriesReports(other.joinRepositoriesReports)
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
    if (IsGitInstalled() == false)
        return new JobStatus(JobStatus::ERROR, errorGitNotInstalled);

    debugManager.SetUse(usingDebugInformation);
    vector<JobStatus*> statuses;
    vector<pair<string, string> >::const_iterator it=folderList.begin();
    for (; it!=folderList.end(); it++)
    {
        JobStatus* repositoryStatus = RunRepositoryBackup(it->first, it->second);
        statuses.push_back(repositoryStatus);
    }

    return debugManager.UpdateStatus(CreateGlobalStatus(statuses));
}

void GitFsBackupJob::SetOutputDebugInformation(const bool value)
{
    usingDebugInformation = value;
}

void GitFsBackupJob::SetJoinAllBackups(const bool value)
{
    joinRepositoriesReports = value;
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
    const string params = string("init ") + path;
    ConsoleJob commandJob("git", params);
    commandJob.RunWithoutStatus();
    debugManager.AddStringDataLine("Create repository params", params);
    debugManager.AddStringDataLine("Create repository output", commandJob.GetCommandOutput());
    debugManager.AddIntDataLine("Create repository value", commandJob.GetCommandReturnCode());
    if (commandJob.GetCommandReturnCode() == 0)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorCreatingRepository);
    }
}

void GitFsBackupJob::CleanDestination(const string &destination, JobStatus *status)
{
    const string params = string("-Rf ") + destination + "*";
    ConsoleJob commandJob("rm", params);
    commandJob.RunWithoutStatus();
    debugManager.AddStringDataLine("Clean params", params);
    debugManager.AddStringDataLine("Clean output", commandJob.GetCommandOutput());
    debugManager.AddIntDataLine("Clean value", commandJob.GetCommandReturnCode());
    if (commandJob.GetCommandReturnCode() == 0)
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
    ConsoleJob commandJob("git", "add -A");
    commandJob.RunWithoutStatus();
    debugManager.AddStringDataLine("Add output", commandJob.GetCommandOutput());
    debugManager.AddIntDataLine("Add value", commandJob.GetCommandReturnCode());
    if (commandJob.GetCommandReturnCode() == 0)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorAddingData);
    }
}

string GitFsBackupJob::CommitData(JobStatus *status)
{
    ConsoleJob commandJob("git", "commit -m \"Automated backup\"");
    commandJob.RunWithoutStatus();
    debugManager.AddStringDataLine("Commit output", commandJob.GetCommandOutput());
    debugManager.AddIntDataLine("Commit value", commandJob.GetCommandReturnCode());
    if (commandJob.GetCommandReturnCode() == 0)
        return GetCommitId(commandJob.GetCommandOutput());
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
    const string command = (isTargetLocal ? "cp" : "scp");
    string params;
    if (isTargetLocal)
        params = "-R ";
    else
        params = string("-r ") + sshUser + "@" + sshHost + ":";
    params += source + "* " + destination;

    ConsoleJob commandJob(command, params);
    commandJob.RunWithoutStatus();

    debugManager.AddStringDataLine("Copy command", command + " " + params);
    debugManager.AddStringDataLine("Copy output", commandJob.GetCommandOutput());
    debugManager.AddIntDataLine("Copy value", commandJob.GetCommandReturnCode());

    return commandJob.GetCommandReturnCode();
}

int GitFsBackupJob::GetRevisionCount() const
{
    ConsoleJob commandJob("git", "rev-list --all --count");
    commandJob.RunWithoutStatus();
    return (commandJob.GetCommandReturnCode() == 0) ? atoi(commandJob.GetCommandOutput().c_str()) : -1;
}

void GitFsBackupJob::CreateInitialReport(JobStatus *status)
{
    ConsoleJob commandJob("ls");
    commandJob.RunWithoutStatus();
    if (commandJob.GetCommandReturnCode() != 0)
    {
        status->SetCode(JobStatus::OK_WITH_WARNINGS);
        status->SetDescription("Report creation failed");
        status->AddFileBuffer("GitFsBackup.txt", commandJob.GetCommandOutput());
    }
    else
    {
        vector<string> fileList;
        Tools::TokenizeString(commandJob.GetCommandOutput(), '\n', fileList);
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
    const string params = string("diff-tree --no-commit-id --name-status -r ") + commitId;
    ConsoleJob commandJob("git", params);
    commandJob.RunWithoutStatus();
    if (commandJob.GetCommandReturnCode() == 0)
    {
        GitCommitReportParser parser;
        bool ok = parser.ParseBuffer(commandJob.GetCommandOutput());
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

bool GitFsBackupJob::IsGitInstalled() const
{
    ConsoleJob checkGitCommand("git", "--version");
    checkGitCommand.RunWithoutStatus();
    return (checkGitCommand.GetCommandReturnCode() == 0);
}

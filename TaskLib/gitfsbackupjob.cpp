#include "gitfsbackupjob.h"

#include <stdlib.h>
#include <unistd.h>
#include <sstream>

#include "consolejob.h"
#include "copyjobchooser.h"
#include "filetools.h"
#include "gitcommitreportparser.h"
#include "rawcopyfsbackupjob.h"
#include "tools.h"

using namespace std;

const string errorCreatingRepository = "Backup repository was not created";
const string errorCleaning = "Failed cleaning old data";
const string errorCopyingData = "Data was not properly copied";
const string errorAddingData = "Git add failed";
const string errorCommitingData = "Git commit failed";
const string errorGitNotInstalled = "Git not installed";

const int emptyDirError = 1;
const int gitNothingToCommitWarningCode = 1;
const int gitCommitUtf8WarningCode = 137;

GitFsBackupJob::GitFsBackupJob()
    : AbstractBackupJob("GitFsBackup"),
      forceRawCopy(false)
{
    statusManager.SetDebugManager(&debugManager);
}

GitFsBackupJob::GitFsBackupJob(const GitFsBackupJob &other)
    : AbstractBackupJob(other), statusManager(other.statusManager),
      forceRawCopy(other.forceRawCopy)
{
    statusManager.SetDebugManager(&debugManager);
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
    else
        return AbstractBackupJob::Run();
}

void GitFsBackupJob::SetOutputDebugInformation(const bool value)
{
    debugManager.SetUse(value);
}

void GitFsBackupJob::SetJoinAllBackups(const bool value)
{
    statusManager.SetJoinReports(value);
}

void GitFsBackupJob::SetForceRawCopyUse(const bool value)
{
    forceRawCopy = value;
}

void GitFsBackupJob::RunRepositoryBackup(const string &source,
                                         const string &destination,
                                         ResultCollection& results)
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

    FileBackupReport* report = new FileBackupReport();
    if (HasChangesInRepository())
    {
        if (status->GetCode() == JobStatus::OK)
            AddData(status);

        string commitId("");
        if (status->GetCode() == JobStatus::OK)
            commitId = CommitData(status);

        if (status->GetCode() == JobStatus::OK)
            CreateReport(commitId, status, *report);
    }

    chdir(originalDirectory.c_str());

    results.push_back(make_pair(status, report));
}

JobStatus *GitFsBackupJob::CreateGlobalStatus(const ResultCollection& results)
{
    return statusManager.CreateGlobalStatus(results, folderList);
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
    AbstractCopyFsBackupJob* copyJob = (forceRawCopy) ? new RawCopyFsBackupJob()
                                                      : CopyJobChooser::GetBestAvailable();

    if (isTargetLocal)
        copyJob->SetTargetLocal();
    else
        copyJob->SetTargetRemote(sshUser, sshHost);

    int returnValue = copyJob->RunOnParameters(source, destination);
    if (returnValue == 0 || returnValue == emptyDirError)
        status->SetCode(JobStatus::OK);
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorCopyingData);
    }

    delete copyJob;
}

void GitFsBackupJob::AddData(JobStatus *status)
{
    FixCRLFIssue();

    ConsoleJob commandJob("git", "add -A :/");
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
    SetGitUserToFixUtf8Warning();

    ConsoleJob commandJob("git", "commit -m \"Automated backup\"");
    commandJob.RunWithoutStatus();
    debugManager.AddStringDataLine("Commit params", commandJob.GetCommandParameters());
    debugManager.AddStringDataLine("Commit output", commandJob.GetCommandOutput());
    debugManager.AddIntDataLine("Commit value", commandJob.GetCommandReturnCode());
    if (IsCommitCodeOk(commandJob.GetCommandReturnCode()))
        return GetCommitId(commandJob.GetCommandOutput());
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(errorCommitingData);
        return string("");
    }
}

bool GitFsBackupJob::HasChangesInRepository() const
{
    ConsoleJob commandJob("git", "status --porcelain");
    commandJob.RunWithoutStatus();
    return (commandJob.GetCommandOutput() != "");
}

void GitFsBackupJob::CreateReport(const string& commitId, JobStatus *status,
                                  FileBackupReport& report)
{
    int revisionCount = GetRevisionCount();
    if (revisionCount < 2)
        CreateInitialReport(status, report);
    else
        CreateDifferentialReport(commitId, status, report);
}

void GitFsBackupJob::FixCRLFIssue()
{
    ConsoleJob::Run("git", "config core.autocrlf false");
    ConsoleJob::Run("git", "config core.safecrlf false");
}

void GitFsBackupJob::SetGitUserToFixUtf8Warning()
{
    ConsoleJob::Run("git", "config user.name \"TaskManager\"");
    ConsoleJob::Run("git", "config user.email task@manager.com");
}

int GitFsBackupJob::GetRevisionCount() const
{
    ConsoleJob commandJob("git", "rev-list --all --count");
    commandJob.RunWithoutStatus();
    return (commandJob.GetCommandReturnCode() == 0) ? atoi(commandJob.GetCommandOutput().c_str()) : -1;
}

void GitFsBackupJob::CreateInitialReport(JobStatus *status, FileBackupReport &report)
{
    debugManager.AddStringDataLine("Creating Initial Report", "");
    ConsoleJob commandJob("ls");
    commandJob.RunWithoutStatus();
    if (commandJob.GetCommandReturnCode() != 0)
    {
        status->SetCode(JobStatus::OK_WITH_WARNINGS);
        status->AddFileBuffer("output", commandJob.GetCommandOutput());
    }
    else
    {
        vector<string> fileList;
        Tools::TokenizeString(commandJob.GetCommandOutput(), '\n', fileList);

        vector<string>::const_iterator it = fileList.begin();
        for(; it!=fileList.end(); ++it)
            report.AddAsAdded(*it);
        status->SetCode(JobStatus::OK);
    }
}

void GitFsBackupJob::CreateDifferentialReport(const string &commitId, JobStatus *status,
                                              FileBackupReport& report)
{
    debugManager.AddStringDataLine("Creating Differential Report", "");
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
            parser.GetReport(report);
            return;
        }
    }

    status->SetCode(JobStatus::OK_WITH_WARNINGS);
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

bool GitFsBackupJob::IsCommitCodeOk(const int code) const
{
    return (code == 0 || code == gitCommitUtf8WarningCode);
}

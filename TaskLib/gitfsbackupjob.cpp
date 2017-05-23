#include "gitfsbackupjob.h"

#include <stdlib.h>
#include <unistd.h>
#include <sstream>

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
const int gitNothingToCommitWarningCode = 1;
const int gitCommitUtf8WarningCode = 137;

GitFsBackupJob::GitFsBackupJob()
    : AbstractBackupJob("GitFsBackup"),
      joinRepositoriesReports(true)
{
}

GitFsBackupJob::GitFsBackupJob(const GitFsBackupJob &other)
    : AbstractBackupJob(other),
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
    else
        return AbstractBackupJob::Run();
}

void GitFsBackupJob::SetOutputDebugInformation(const bool value)
{
    debugManager.SetUse(value);
}

void GitFsBackupJob::SetJoinAllBackups(const bool value)
{
    joinRepositoriesReports = value;
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

JobStatus *GitFsBackupJob::CreateGlobalStatus(const ResultCollection& statuses)
{
    debugManager.AddIntDataLine("Statuses to handle", statuses.size());
    if (statuses.size() == 1)
        return CreateSingleStatus(statuses);
    else
    {
        if (AreAllStatusesEqual(statuses, JobStatus::OK))
            return CreateAllOkStatus(statuses);
        else
            return CreateMixedStatus(statuses);
    }
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
    FixCRLFIssue();

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

int GitFsBackupJob::RunCopyCommand(const string &source, const string &destination)
{
    // @TODO : check for invalid source and return an error code for this case
    // (different than emptyDirError!)

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

bool GitFsBackupJob::AreAllStatusesEqual(const vector<pair<JobStatus*, FileBackupReport*> > &statuses,
                                         const int expectedCode)
{
    bool areAllOk = true;
    vector<pair<JobStatus*, FileBackupReport*> >::const_iterator it = statuses.begin();
    for (; it != statuses.end(); ++it)
    {
        if (it->first->GetCode() != expectedCode)
        {
            areAllOk = false;
            break;
        }
    }

    return areAllOk;
}

JobStatus *GitFsBackupJob::CreateSingleStatus(
                const std::vector<std::pair<JobStatus *, FileBackupReport *> > &statuses)
{
    debugManager.AddStringDataLine("Creating Single status", "");
    JobStatus* status = new JobStatus(statuses.front().first->GetCode());
    status->SetDescription(statuses.front().second->GetMiniDescription());
    status->AddFileBuffer("GitFsBackup.txt", statuses.front().second->GetFullDescription());
    return status;
}

JobStatus *GitFsBackupJob::CreateAllOkStatus(const std::vector<std::pair<JobStatus *,
                                             FileBackupReport *> > &statuses)
{
    JobStatus* status = new JobStatus(JobStatus::OK);

    if (joinRepositoriesReports)
        return CreateJoinedStatus(statuses);
    else
        return CreateSeparatedStatus(statuses);

    return status;
}

JobStatus *GitFsBackupJob::CreateMixedStatus(const std::vector<std::pair<JobStatus *, FileBackupReport *> > &statuses)
{
    debugManager.AddStringDataLine("Creating Mixed status", "");
    JobStatus* status = new JobStatus(JobStatus::ERROR);
    status->SetDescription(CreateFoldersMiniDescription(statuses));
    status->AddFileBuffer("GitFsBackup.txt", CreateStatusesDescription(statuses));
    return status;
}

JobStatus *GitFsBackupJob::CreateJoinedStatus(const std::vector<std::pair<JobStatus *, FileBackupReport *> > &statuses)
{
    debugManager.AddStringDataLine("Creating Joined status", "");
    JobStatus* status = new JobStatus(JobStatus::OK);
    FileBackupReport globalReport;
    vector<pair<string,string> >::const_iterator itDestination = folderList.begin();
    vector<pair<JobStatus *, FileBackupReport *> >::const_iterator it = statuses.begin();
    for (; it!=statuses.end(); ++it, ++itDestination)
        globalReport.AddWithPrefix(*it->second, itDestination->second);

    status->SetDescription(globalReport.GetMiniDescription());
    status->AddFileBuffer("GitFsBackup.txt", globalReport.GetFullDescription());
    return status;
}

JobStatus *GitFsBackupJob::CreateSeparatedStatus(const std::vector<std::pair<JobStatus *, FileBackupReport *> > &statuses)
{
    debugManager.AddStringDataLine("Creating Separated status", "");
    JobStatus* status = new JobStatus(JobStatus::OK);
    FileBackupReport globalReport;
    vector<pair<JobStatus *, FileBackupReport *> >::const_iterator it = statuses.begin();
    for (; it!=statuses.end(); ++it)
        globalReport.Add(*it->second);

    status->SetDescription(globalReport.GetMiniDescription());
    status->AddFileBuffer("GitFsBackup.txt", CreateStatusesDescription(statuses));
    return status;
}

string GitFsBackupJob::CreateStatusesDescription(const std::vector<std::pair<JobStatus *, FileBackupReport *> > &statuses)
{
    string fullDescription("");
    vector<pair<string,string> >::const_iterator itDestination = folderList.begin();
    vector<pair<JobStatus *, FileBackupReport *> >::const_iterator it = statuses.begin();
    for (; it!=statuses.end(); ++it, ++itDestination)
    {
        fullDescription += BuildRepositoryHeader(itDestination->second);
        fullDescription += it->second->GetFullDescription();
    }
    fullDescription += BuildFooter();
    return fullDescription;
}

string GitFsBackupJob::CreateFoldersMiniDescription(const std::vector<std::pair<JobStatus *, FileBackupReport *> > &statuses)
{
    const int successCount = ComputeSuccessCount(statuses);
    const int failureCount = statuses.size() - successCount;

    stringstream miniDescription;
    if (successCount > 0)
    {
        miniDescription << successCount << " succeeded";
        miniDescription << ((failureCount > 0) ? ", " : ".");
    }

    if (failureCount > 0)
        miniDescription << failureCount << "failed.";

    return miniDescription.str();
}

int GitFsBackupJob::ComputeSuccessCount(const std::vector<std::pair<JobStatus *, FileBackupReport *> > &statuses) const
{
    int count = 0;
    vector<pair<JobStatus *, FileBackupReport *> >::const_iterator it = statuses.begin();
    for (; it!=statuses.end(); ++it)
    {
        if (it->first->GetCode() == JobStatus::OK)
            ++count;
    }
    return count;
}

string GitFsBackupJob::BuildRepositoryHeader(const string& name)
{
    return string("--- ") + name + " ---\n";
}

string GitFsBackupJob::BuildFooter()
{
    return string("------------------\n");
}

bool GitFsBackupJob::IsCommitCodeOk(const int code) const
{
    return (code == 0 || code == gitCommitUtf8WarningCode);
}

#include "gitfsbackupjob.h"

#include <stdlib.h>
#include <unistd.h>
#include <sstream>

#include "consolejob.h"
#include "copyjobchooser.h"
#include "filetools.h"
#include "gitcommitreportparser.h"
#include "rawcopyfsbackupjob.h"
#include "rsynccopyfsbackupjob.h"
#include "tools.h"

using namespace std;

const string errorCreatingRepository = "Backup repository was not created";
const string errorCleaning = "Failed cleaning old data";
const string errorCopyingData = "Data was not properly copied";
const string errorAddingData = "Git add failed";
const string errorCommitingData = "Git commit failed";
const string errorGitNotInstalled = "Git not installed";

static const int emptyDirError = 1;
static const int gitNothingToCommitWarningCode = 1;
static const int gitCommitUtf8WarningCode = 137;
static const int gitNotConfiguredError = 128;

static const string gitUserName = "TaskManager";
static const string gitUserEmail = "task@manager.com";

GitFsBackupJob::GitFsBackupJob()
    : AbstractBackupJob(), forceRawCopy(false)
{
}

GitFsBackupJob::GitFsBackupJob(const GitFsBackupJob &other)
    : AbstractBackupJob(other), forceRawCopy(other.forceRawCopy)
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

void GitFsBackupJob::SetJoinAllBackups(const bool value)
{
    statusManager->SetJoinReports(value);
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
        CopyData(source, destination, status);

    string originalDirectory = FileTools::GetCurrentFullPath();
    chdir(destination.c_str());

    FileBackupReport* report = new FileBackupReport();
    const bool hasChanges = HasChangesInRepository();
    debugManager->AddDataLine<bool>("Changes detected", hasChanges);
    if (hasChanges)
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

void GitFsBackupJob::CreateGitRepository(const string &path, JobStatus *status)
{
    debugManager->AddTagLine("Creating Repository");
    SetGitUserToFixUtf8Warning();
    FixCRLFIssue();

    const string params = string("init ") + path;
    ConsoleJob commandJob("git", params);
    commandJob.RunWithoutStatus();
    debugManager->AddDataLine<string>("Create repository params", params);
    debugManager->AddDataLine<string>("Create repository output", commandJob.GetCommandOutput());
    debugManager->AddDataLine<int>("Create repository value", commandJob.GetCommandReturnCode());
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
    debugManager->AddTagLine("Cleaning destination");

    string params = destination + " -mindepth 1 -path \"" + destination + ".git\" ";
    params += "-prune -o -print0 | xargs -0 rm -Rf";

    // @warning This doesn't work in BusyBox (synology system), as find command is too simple there.
    ConsoleJob commandJob("find", params);
    commandJob.RunWithoutStatus();
    debugManager->AddDataLine<string>("Clean params", params);
    debugManager->AddDataLine<string>("Clean output", commandJob.GetCommandOutput());
    debugManager->AddDataLine<int>("Clean value", commandJob.GetCommandReturnCode());
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
    debugManager->AddTagLine("Copying data");
    AbstractCopyFsBackupJob* copyJob = PrepareCopy(destination, status);
    if (status->GetCode() == JobStatus::OK)
        RunCopy(copyJob, source, destination, status);
    delete copyJob;
}

void GitFsBackupJob::AddData(JobStatus *status)
{
    debugManager->AddTagLine("Adding data to git");
    ConsoleJob commandJob("git", "add -A :/");
    commandJob.RunWithoutStatus();
    debugManager->AddDataLine<string>("Add output", commandJob.GetCommandOutput());
    debugManager->AddDataLine<int>("Add value", commandJob.GetCommandReturnCode());
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
    debugManager->AddTagLine("Committing data to git");
    ConsoleJob commandJob("git", "commit -m \"Automated backup\"");
    commandJob.RunWithoutStatus();
    LogDebugCommand("Commit", commandJob);
    if (commandJob.GetCommandReturnCode() == gitNotConfiguredError)
    {
        ConfigureGitRepository();
        commandJob.RunWithoutStatus();
        LogDebugCommand("Commit", commandJob);
    }

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
    ConsoleJob::Run("git", "config --local core.autocrlf false");
    ConsoleJob::Run("git", "config --local core.safecrlf false");
}

void GitFsBackupJob::SetGitUserToFixUtf8Warning()
{
    ConsoleJob::Run("git", "config --local user.name \"TaskManager\"");
    ConsoleJob::Run("git", "config --local user.email task@manager.com");
}

int GitFsBackupJob::GetRevisionCount() const
{
    ConsoleJob commandJob("git", "rev-list --all --count");
    commandJob.RunWithoutStatus();
    return (commandJob.GetCommandReturnCode() == 0) ? atoi(commandJob.GetCommandOutput().c_str()) : -1;
}

void GitFsBackupJob::CreateInitialReport(JobStatus *status, FileBackupReport &report)
{
    debugManager->AddDataLine<string>("Creating Initial Report", "");
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
    debugManager->AddDataLine<string>("Creating Differential Report", "");
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

AbstractCopyFsBackupJob *GitFsBackupJob::PrepareCopy(const string &destination, JobStatus *status)
{
    bool usingRawCopy = (forceRawCopy || !RsyncCopyFsBackupJob::IsAvailable());

    if (debugManager->GetUse() == DebugOutput::ALWAYS)
    {
        string rsyncPath = Tools::GetCommandPath("rsync", ConsoleJob::appSearchPaths);
        debugManager->AddDataLine<string>("Rsync path", rsyncPath);
        const bool manualCondition = (rsyncPath != "");
        debugManager->AddDataLine<bool>("Manual condition", manualCondition);
    }

    debugManager->AddDataLine<bool>("Force Raw copy", forceRawCopy);
    debugManager->AddDataLine<bool>("Using Raw copy", usingRawCopy);

    AbstractCopyFsBackupJob* copyJob;
    if (usingRawCopy)
        copyJob = new RawCopyFsBackupJob();
    else
        copyJob = new RsyncCopyFsBackupJob();

    copyJob->SetParentDebugManager(debugManager);

    if (usingRawCopy)
        CleanDestination(destination, status);
    else
    {
        RsyncCopyFsBackupJob* castJob = static_cast<RsyncCopyFsBackupJob*>(copyJob);
        castJob->AddToExclusions(".git/");
    }
    return copyJob;
}

void GitFsBackupJob::RunCopy(AbstractCopyFsBackupJob *copyJob, const string &source,
                             const string &destination, JobStatus *status)
{
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
}

void GitFsBackupJob::LogDebugCommand(const string &title, const ConsoleJob &job)
{
    debugManager->AddDataLine<string>(title + " command", job.GetCommand());
    debugManager->AddDataLine<string>(title + " params", job.GetCommandParameters());
    debugManager->AddDataLine<string>(title + " output", job.GetCommandOutput());
    debugManager->AddDataLine<int>(title + " return value", job.GetCommandReturnCode());
}

bool GitFsBackupJob::ConfigureGitRepository()
{
    if (!SetupGitConfig("email", gitUserEmail))
        return false;

    if (!SetupGitConfig("name", gitUserName))
        return false;

    return true;
}

bool GitFsBackupJob::SetupGitConfig(const string &configuration, const string &value)
{
    // TODO wait for production run to confirm correct error message before activating implementation
    return false;

/*    const string param = string("config user.") + configuration + " \"" + value + "\"";
    ConsoleJob job("git", param);
    job.RunWithoutStatus();
    if (!job.IsRunOk())
        LogDebugCommand(string("Git config ") + configuration, job);

    return job.IsRunOk();*/
}

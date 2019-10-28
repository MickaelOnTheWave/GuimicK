#include "gitfsbackupjob.h"

#include <stdlib.h>
#include <sstream>

#include "consolejob.h"
#include "copyjobchooser.h"
#include "filetools.h"
#include "gitplumbingreportparser.h"
#include "gitcommontools.h"
#include "linuxcopyfsbackupjob.h"
#include "pathtools.h"
#include "rsynccopyfsbackupjob.h"
#include "stringtools.h"
#include "stringtools.h"

using namespace std;

const wstring errorCreatingRepository  = L"Backup repository was not created";
const wstring errorCleaning            = L"Failed cleaning old data";
const wstring errorCopyingData         = L"Data was not properly copied";
const wstring errorAddingData          = L"Git add failed";
const wstring errorCommitingData       = L"Git commit failed";
const wstring errorGitNotInstalled     = L"Git not installed";

static const int emptyDirError = 1;
static const int gitNothingToCommitWarningCode = 1;
static const int gitCommitUtf8WarningCode = 137;
static const int gitNotConfiguredError = 128;

static const wstring gitUserName    = L"TaskManager";
static const wstring gitUserEmail   = L"task@manager.com";
static const wstring defaultName    = L"Git Filesystem Backup";

GitFsBackupJob::GitFsBackupJob()
    : AbstractBackupJob(defaultName), forceRawCopy(false)
{
}

GitFsBackupJob::GitFsBackupJob(const GitFsBackupJob &other)
    : AbstractBackupJob(other), forceRawCopy(other.forceRawCopy)
{

}

AbstractJob *GitFsBackupJob::Clone()
{
    return new GitFsBackupJob(*this);
}

JobStatus *GitFsBackupJob::Run()
{
    if (IsTargetLocal() && IsGitInstalled() == false)
        return new JobStatus(JobStatus::Error, errorGitNotInstalled);
    else
       return AbstractBackupJob::Run();
}

wstring GitFsBackupJob::GetTypeName() const
{
   return defaultName;
}

bool GitFsBackupJob::GetForceRawCopyUse() const
{
   return forceRawCopy;
}

void GitFsBackupJob::SetForceRawCopyUse(const bool value)
{
   forceRawCopy = value;
}

JobStatus* GitFsBackupJob::RestoreBackupFromServer(const wstring& source,
                                         const wstring& destination)
{
   wstring originalDirectory = PathTools::GetCurrentFullPath();
   JobStatus* status = GitCommonTools::ChangeCurrentDir(source);
   if (status->IsOk())
   {
      RunGitExport(originalDirectory + L"/" + destination, status);
      GitCommonTools::ChangeCurrentDir(originalDirectory);
   }

   return status;
}

void GitFsBackupJob::RunRepositoryBackup(const wstring &source,
                                         const wstring &destination,
                                         ResultCollection& results)
{
   const wstring fullDestination = repository + destination;
    JobStatus* status = new JobStatus(JobStatus::Ok);
    if (FileTools::FolderExists(fullDestination) == false)
        CreateGitRepository(fullDestination, status);

    if (status->GetCode() == JobStatus::Ok)
        CopyData(source, fullDestination, status);

    wstring originalDirectory = PathTools::GetCurrentFullPath();
    bool ok = GitCommonTools::ChangeCurrentDir(fullDestination, results);
    if (!ok)
        return;

    FileBackupReport* report = new FileBackupReport();
    const bool hasChanges = HasChangesInRepository();
    debugManager->AddDataLine<bool>(L"Changes detected", hasChanges);
    if (hasChanges)
    {
        if (status->GetCode() == JobStatus::Ok)
            AddData(status);

        wstring commitId(L"");
        if (status->GetCode() == JobStatus::Ok)
            commitId = CommitData(status);

        if (status->GetCode() == JobStatus::Ok)
            CreateReport(commitId, status, *report);
    }

    GitCommonTools::ChangeCurrentDir(originalDirectory, results);

    results.push_back(make_pair(status, report));
}

void GitFsBackupJob::CreateGitRepository(const wstring &path, JobStatus *status)
{
    debugManager->AddTagLine(L"Creating Repository");
    SetGitUserToFixUtf8Warning();
    FixCRLFIssue();

    const wstring params = wstring(L"init ") + path;
    ConsoleJob commandJob(L"git", params);
    commandJob.RunWithoutStatus();
    debugManager->AddDataLine<wstring>(L"Create repository params", params);
    debugManager->AddDataLine<wstring>(L"Create repository output", commandJob.GetCommandOutput());
    debugManager->AddDataLine<int>(L"Create repository value", commandJob.GetCommandReturnCode());
    if (commandJob.GetCommandReturnCode() == 0)
        status->SetCode(JobStatus::Ok);
    else
    {
        status->SetCode(JobStatus::Error);
        status->SetDescription(errorCreatingRepository);
    }
}

void GitFsBackupJob::CleanDestination(const wstring &destination, JobStatus *status)
{
    debugManager->AddTagLine(L"Cleaning destination");

    wstring params = destination + L" -mindepth 1 -path \"" + destination + L".git\" ";
    params += L"-prune -o -print0 | xargs -0 rm -Rf";

    // @warning This doesn't work in BusyBox (synology system), as find command is too simple there.
    ConsoleJob commandJob(L"find", params);
    commandJob.RunWithoutStatus();
    debugManager->AddDataLine<wstring>(L"Clean params", params);
    debugManager->AddDataLine<wstring>(L"Clean output", commandJob.GetCommandOutput());
    debugManager->AddDataLine<int>(L"Clean value", commandJob.GetCommandReturnCode());
    if (commandJob.GetCommandReturnCode() == 0)
        status->SetCode(JobStatus::Ok);
    else
    {
        status->SetCode(JobStatus::Error);
        status->SetDescription(errorCleaning);
    }
}

void GitFsBackupJob::CopyData(const wstring &source, const wstring &destination,
                              JobStatus *status)
{
    debugManager->AddTagLine(L"Copying data");
    AbstractCopyFsBackupJob* copyJob = PrepareCopy(destination, status);
    if (status->GetCode() == JobStatus::Ok)
        RunCopy(copyJob, source, destination, status);
    delete copyJob;
}

void GitFsBackupJob::AddData(JobStatus *status)
{
    debugManager->AddTagLine(L"Adding data to git");
    ConsoleJob commandJob(L"git", L"add -A :/");
    commandJob.RunWithoutStatus();
    debugManager->AddDataLine<wstring>(L"Add output", commandJob.GetCommandOutput());
    debugManager->AddDataLine<int>(L"Add value", commandJob.GetCommandReturnCode());
    if (commandJob.GetCommandReturnCode() == 0)
        status->SetCode(JobStatus::Ok);
    else
    {
        status->SetCode(JobStatus::Error);
        status->SetDescription(errorAddingData);
    }
}

wstring GitFsBackupJob::CommitData(JobStatus *status)
{
    debugManager->AddTagLine(L"Committing data to git");
    ConsoleJob commandJob(L"git", L"commit -m \"Automated backup\"");
    commandJob.RunWithoutStatus();
    LogDebugCommand(L"Commit", commandJob);
    if (commandJob.GetCommandReturnCode() == gitNotConfiguredError)
    {
        ConfigureGitRepository();
        commandJob.RunWithoutStatus();
        LogDebugCommand(L"Commit", commandJob);
    }

    if (IsCommitCodeOk(commandJob.GetCommandReturnCode()))
        return GetCommitId(commandJob.GetCommandOutput());
    else
    {
        status->SetCode(JobStatus::Error);
        status->SetDescription(errorCommitingData);
        return wstring(L"");
    }
}

bool GitFsBackupJob::HasChangesInRepository() const
{
    ConsoleJob commandJob(L"git", L"status --porcelain");
    commandJob.RunWithoutStatus();
    return (commandJob.GetCommandOutput() != L"");
}

void GitFsBackupJob::CreateReport(const wstring& commitId, JobStatus *status,
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
    ConsoleJob::Run(L"git", L"config --local core.autocrlf false");
    ConsoleJob::Run(L"git", L"config --local core.safecrlf false");
}

// TODO : check this. THere was an error that suggested that it is not working
// as expected. There is an improved version of it, ConfigureGitRepository().
void GitFsBackupJob::SetGitUserToFixUtf8Warning()
{
    ConsoleJob::Run(L"git", L"config --local user.name \"TaskManager\"");
    ConsoleJob::Run(L"git", L"config --local user.email task@manager.com");
}

int GitFsBackupJob::GetRevisionCount() const
{
    ConsoleJob commandJob(L"git", L"rev-list --all --count");
    commandJob.RunWithoutStatus();
    if (commandJob.GetCommandReturnCode() == 0)
       return StringTools::ToInt(commandJob.GetCommandOutput());
    else
       return -1;
}

void GitFsBackupJob::CreateInitialReport(JobStatus *status, FileBackupReport &report)
{
    debugManager->AddDataLine<wstring>(L"Creating Initial Report", L"");
    ConsoleJob commandJob(L"find", L". -path ./.git -prune -o -print");
    commandJob.RunWithoutStatus();
    LogDebugCommand(L"Initial report command", commandJob);
    if (commandJob.GetCommandReturnCode() != 0)
    {
        status->SetCode(JobStatus::OkWithWarnings);

        wstring content = L"Error while trying to create report. Here is output data:\n";
        content += commandJob.GetCommandOutput();
        status->AddFileBuffer(GetAttachmentName(), content);
    }
    else
    {
        // TODO : put this in a class and create a test suite for it.
        // There is already production.txt file to check on test.
        vector<wstring> fileList;
        StringTools::Tokenize(commandJob.GetCommandOutput(), '\n', fileList);

        vector<wstring>::const_iterator it = fileList.begin();
        for(; it!=fileList.end(); ++it)
        {
            if (*it != L".")
                report.AddAsAdded(CreateFilteredFileName(*it));
        }
        status->SetCode(JobStatus::Ok);
    }
}

void GitFsBackupJob::CreateDifferentialReport(const wstring &commitId, JobStatus *status,
                                              FileBackupReport& report)
{
    debugManager->AddDataLine<wstring>(L"Creating Differential Report", L"");
    const wstring params = wstring(L"diff-tree --no-commit-id --name-status -r ") + commitId;
    ConsoleJob commandJob(L"git", params);
    commandJob.RunWithoutStatus();
    if (commandJob.GetCommandReturnCode() == 0)
    {
        GitPlumbingReportParser parser;
        bool ok = parser.ParseBuffer(commandJob.GetCommandOutput());
        if (ok)
        {
            status->SetCode(JobStatus::Ok);
            parser.GetReport(report);
            return;
        }
    }

    status->SetCode(JobStatus::OkWithWarnings);
}

wstring GitFsBackupJob::GetCommitId(const wstring &output)
{
    wstring commitwstring = output.substr(0, output.find_first_of(']'));
    size_t posStartId = commitwstring.find_last_of(' ');
    return commitwstring.substr(posStartId+1, commitwstring.size()-1);
}

bool GitFsBackupJob::IsGitInstalled() const
{
    ConsoleJob checkGitCommand(L"git", L"--version");
    checkGitCommand.RunWithoutStatus();
    return (checkGitCommand.GetCommandReturnCode() == 0);
}

bool GitFsBackupJob::IsCommitCodeOk(const int code) const
{
    return (code == 0 || code == gitCommitUtf8WarningCode);
}

AbstractCopyFsBackupJob *GitFsBackupJob::PrepareCopy(const wstring &destination, JobStatus *status)
{
    bool usingRawCopy = (forceRawCopy || !RsyncCopyFsBackupJob::IsAvailable());

    if (debugManager->GetUse() == DebugOutput::ALWAYS)
    {
        wstring rsyncPath = PathTools::GetCommandPath(L"rsync", ConsoleJob::appSearchPaths);
        debugManager->AddDataLine<wstring>(L"Rsync path", rsyncPath);
        const bool manualCondition = (rsyncPath != L"");
        debugManager->AddDataLine<bool>(L"Manual condition", manualCondition);
    }

    debugManager->AddDataLine<bool>(L"Force Raw copy", forceRawCopy);
    debugManager->AddDataLine<bool>(L"Using Raw copy", usingRawCopy);

    AbstractCopyFsBackupJob* copyJob;
    if (usingRawCopy)
        copyJob = new LinuxCopyFsBackupJob();
    else
        copyJob = new RsyncCopyFsBackupJob();

    copyJob->SetParentDebugManager(debugManager);

    if (usingRawCopy)
        CleanDestination(destination, status);
    else
    {
        RsyncCopyFsBackupJob* castJob = static_cast<RsyncCopyFsBackupJob*>(copyJob);
        castJob->AddToExclusions(L".git/");
    }
    return copyJob;
}

void GitFsBackupJob::RunCopy(AbstractCopyFsBackupJob *copyJob, const wstring &source,
                             const wstring &destination, JobStatus *status)
{
    if (target.isLocal)
        copyJob->SetTargetLocal();
    else
        copyJob->SetTargetRemote(target.sshUser, target.sshHost);

    int returnValue = copyJob->RunOnParameters(source, destination);   
    if (returnValue == 0 || returnValue == emptyDirError)
        status->SetCode(JobStatus::Ok);
    else
    {
        status->SetCode(JobStatus::Error);
        status->SetDescription(errorCopyingData);
    }
}

void GitFsBackupJob::LogDebugCommand(const wstring &name, const ConsoleJob &job)
{
    debugManager->AddDataLine<wstring>(name + L" command", job.GetCommand());
    debugManager->AddDataLine<wstring>(name + L" params", job.GetCommandParameters());
    debugManager->AddDataLine<wstring>(name + L" output", job.GetCommandOutput());
    debugManager->AddDataLine<int>(name + L" return value", job.GetCommandReturnCode());
}

bool GitFsBackupJob::ConfigureGitRepository()
{
    if (!SetupGitConfig(L"email", gitUserEmail))
        return false;

    if (!SetupGitConfig(L"name", gitUserName))
        return false;

    return true;
}

bool GitFsBackupJob::SetupGitConfig(const wstring &configuration, const wstring &value)
{
    const wstring param = wstring(L"config user.") + configuration + L" \"" + value + L"\"";
    ConsoleJob job(L"git", param);
    job.RunWithoutStatus();
    if (!job.IsRunOk())
        LogDebugCommand(wstring(L"Git config ") + configuration, job);

    return job.IsRunOk();
}

wstring GitFsBackupJob::CreateFilteredFileName(const wstring &name)
{
    if (name.length() < 2)
        return wstring(L"");
    else
       return name.substr(2);
}

void GitFsBackupJob::RunGitExport(const wstring& destination, JobStatus* status)
{
   const wstring params = wstring(L"checkout-index --prefix=") + destination + L" -a";
   ConsoleJob commandJob(L"git", params);
   JobStatus* childStatus = commandJob.Run();
   *status = *childStatus;
   delete childStatus;
}

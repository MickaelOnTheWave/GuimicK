#include "gitbackupjob.h"

#include <sstream>

#include "backupstatusmanager.h"
#include "consolejob.h"
#include "filetools.h"
#include "gitplumbingreportparser.h"
#include "gitcommontools.h"
#include "pathtools.h"

using namespace std;

static const wstring invalidSourceRepositoryError        = L"Invalid source repository";
static const wstring repositoryCloneOk                   = L"Repository cloned successfully";
static const wstring invalidDestinationRepositoryError   = L"Invalid destination repository";
static const wstring fetchUpdateError                    = L"Update error";
static const wstring unknownError                        = L"Unknown error";
static const wstring repositoryPullOk                    = L"Repository successfully updated, see attached file.";
static const wstring reportCreationError                 = L"Failed creating report";

static const wstring repositoryBackupMessage = L"repositories backed up";
static const wstring defaultName = L"Git Repository Backup";

GitBackupJob::GitBackupJob()
 : AbstractBackupJob(defaultName),
   writeLogsToFile(false), archiveContent(L"")
{
    statusManager->SetItemBackupMessage(repositoryBackupMessage);
    statusManager->SetJoinReports(false);
}

GitBackupJob::GitBackupJob(const GitBackupJob &other)
    : AbstractBackupJob(other),
      writeLogsToFile(other.writeLogsToFile),
      archiveContent(other.archiveContent)
{
    statusManager->SetItemBackupMessage(repositoryBackupMessage);
    statusManager->SetJoinReports(false);
}

GitBackupJob::GitBackupJob(const std::vector<std::pair<wstring, wstring> > &repositoryList)
 : AbstractBackupJob(L"Git Repository Backup"),
   writeLogsToFile(false), archiveContent(L"")
{
    folderList = repositoryList;
    statusManager->SetItemBackupMessage(repositoryBackupMessage);
    statusManager->SetJoinReports(false);
}

GitBackupJob::~GitBackupJob()
{
}

AbstractJob *GitBackupJob::Clone()
{
   return new GitBackupJob(*this);
}

wstring GitBackupJob::GetTypeName() const
{
   return defaultName;
}

bool GitBackupJob::GetWriteLogsToFile() const
{
    return writeLogsToFile;
}

void GitBackupJob::SetWriteLogsToFiles(bool enabled)
{
    writeLogsToFile = enabled;
}

void GitBackupJob::RunRepositoryBackup(
        const wstring &source,
        const wstring &destination,
        AbstractBackupJob::ResultCollection& results)
{
    if (FileTools::FolderExists(destination))
        UpdateGitRepository(destination, results);
    else
       RunGitClone(source, destination, results);
}

JobStatus* GitBackupJob::RestoreBackupFromServer(const wstring& source, const wstring& destination)
{
   ConsoleJob* gitCommand = new ConsoleJob(L"git", BuildCloneParameters(source, destination, false));
   JobStatus* status = gitCommand->Run();
   if (gitCommand->GetCommandReturnCode() == 128)
       status->SetDescription(invalidSourceRepositoryError);
   return status;
}

bool GitBackupJob::IsInvalidSourceError(const ConsoleJob &job) const
{
    if (!job.IsRunOk())
    {
        const wstring tagString = L"does not appear to be a git repository";
        if (job.GetCommandOutput().find(tagString) != wstring::npos)
            return true;
    }

    return false;
}

bool GitBackupJob::AreSourcesConsistent() const
{
    vector<pair<wstring, wstring> >::const_iterator it=folderList.begin();
    for (; it!=folderList.end(); it++)
    {
        if (!FileTools::FolderExists(it->first))
            return false;
    }
    return true;
}

void GitBackupJob::UpdateGitRepository(const wstring &gitRepository,
                                       AbstractBackupJob::ResultCollection &statusList)
{
    debugManager->AddDataLine<wstring>(L"Updating Git repository", gitRepository);
    wstring originalDirectory = PathTools::GetCurrentFullPath();

    bool ok = GitCommonTools::ChangeCurrentDir(gitRepository, statusList);
    if (!ok)
        return;

    // TODO : try again to use exceptions here
    const wstring oldHeadId = GetRepositoryHeadId();
    ok = FetchUpdates(gitRepository, statusList);
    if (ok)
    {
        const wstring newHeadId = GetRepositoryHeadId();
        ComputeChanges(gitRepository, oldHeadId, newHeadId, statusList);
    }

    GitCommonTools::ChangeCurrentDir(originalDirectory, statusList);
}

wstring GitBackupJob::GetRepositoryHeadId() const
{
    ConsoleJob command(L"git", L"rev-parse HEAD");
    command.RunWithoutStatus();
    if (command.IsRunOk())
        return command.GetCommandOutput();
    else
        return wstring(L"");
}

bool GitBackupJob::FetchUpdates(const wstring &gitRepository,
                                ResultCollection &statusList)
{
    ConsoleJob command(L"git", L"remote update");
    command.RunWithoutStatus();
    debugManager->AddDataLine<wstring>(L"\tFechUpdate output", command.GetCommandOutput());
    debugManager->AddDataLine<int>(L"\tFechUpdate return code", command.GetCommandReturnCode());
    if (!command.IsRunOk())
    {
        wstring errorMessage;
        if (IsInvalidSourceError(command))
            errorMessage = invalidSourceRepositoryError;
        else
            errorMessage = fetchUpdateError;

        JobStatus* status = new JobStatus(JobStatus::Error, invalidSourceRepositoryError);
        statusList.push_back(make_pair(status, new FileBackupReport()));
        AddToAttachedArchive(gitRepository, command.GetCommandOutput());
    }

    return command.IsRunOk();
}

void GitBackupJob::ComputeChanges(
        const wstring& gitRepository,
        const wstring &oldCommitId, const wstring &newCommitId,
        AbstractBackupJob::ResultCollection &statusList)
{
    wstring params = L"diff --name-status ";
    params += oldCommitId + L" " + newCommitId;
    ConsoleJob command(L"git", params);
    command.RunWithoutStatus();

    debugManager->AddDataLine<wstring>(L"\tComputeChanges output", command.GetCommandOutput());
    debugManager->AddDataLine<int>(L"\tComputeChanges return code", command.GetCommandReturnCode());

    if (command.IsRunOk())
        CreateReport(gitRepository, command.GetCommandOutput(), statusList);
    else
        throw command.GetCommandOutput();
}

void GitBackupJob::CreateReport(
        const std::wstring& gitRepository,
        const std::wstring& commandOutput,
        AbstractBackupJob::ResultCollection &statusList)
{
    GitPlumbingReportParser parser;
    bool ok = parser.ParseBuffer(commandOutput);
    debugManager->AddDataLine<bool>(L"Parser result", ok);
    if (ok)
    {
        FileBackupReport* report = new FileBackupReport();
        parser.GetReport(*report);
        JobStatus* status = new JobStatus(JobStatus::Ok, parser.GetMiniDescription());
        AddToAttachedArchive(gitRepository, report->GetFullDescription());
        statusList.push_back(make_pair(status, report));
    }
    else
    {
        debugManager->AddDataLine<wstring>(L"Parser input", commandOutput);
        JobStatus* status = new JobStatus(JobStatus::OkWithWarnings, reportCreationError);
        AddToAttachedArchive(gitRepository, commandOutput);
        statusList.push_back(make_pair(status, static_cast<FileBackupReport*>(NULL)));
    }
}

void GitBackupJob::AddToAttachedArchive(
        const std::wstring& gitRepository,
        const std::wstring& content)
{
    const wstring repositoryName = FileTools::GetFilenameFromUnixPath(gitRepository);
    archiveContent += wstring(L"\nReport for ") + repositoryName + L"\n";
    archiveContent += content;
}

void GitBackupJob::RunGitClone(const wstring &source,
                               const wstring &destination,
                               ResultCollection &statusList)
{
    debugManager->AddDataLine<wstring>(L"Git Clone on", repository);
    ConsoleJob* gitCommand = new ConsoleJob(L"git", BuildCloneParameters(source, destination, true));
    JobStatus* status = gitCommand->Run();
    debugManager->AddDataLine<int>(L"Clone result", gitCommand->GetCommandReturnCode());
    debugManager->AddDataLine<wstring>(L"Clone output", gitCommand->GetCommandOutput());
    if (gitCommand->GetCommandReturnCode() == 128)
        status->SetDescription(invalidSourceRepositoryError);
    else
    {
        status->SetDescription(repositoryCloneOk);
        AddToAttachedArchive(source, L"Repository cloned");
    }

    statusList.push_back(make_pair(status, static_cast<FileBackupReport*>(NULL)));
    delete gitCommand;
}

wstring GitBackupJob::BuildCloneParameters(const wstring &source, const wstring &destination,
                                          const bool mirror) const
{
    wstring params(L"clone ");
    if (mirror)
       params += L"--mirror ";

    if (IsTargetLocal())
        params += source;
    else
        params += target.sshUser + L"@" + target.sshHost + L":" + source;

    params += wstring(L" 2>&1 ") + destination;
    return params;
}

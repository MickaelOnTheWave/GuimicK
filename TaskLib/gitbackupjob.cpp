#include "gitbackupjob.h"

#include <filetools.h>
#include <sstream>
#include <unistd.h>

#include "backupstatusmanager.h"
#include "consolejob.h"
#include "gitplumbingreportparser.h"
#include "gitcommontools.h"

using namespace std;

static const string invalidSourceRepositoryError        = "Invalid source repository";
static const string repositoryCloneOk                   = "Repository cloned successfully";
static const string invalidDestinationRepositoryError   = "Invalid destination repository";
static const string fetchUpdateError                    = "Update error";
static const string unknownError                        = "Unknown error";
static const string repositoryPullOk                    = "Repository successfully updated, see attached file.";
static const string reportCreationError                 = "Failed creating report";

static const string repositoryBackupMessage = "repositories backed up";
static const string defaultName = "Git Repository Backup";

GitBackupJob::GitBackupJob()
 : AbstractBackupJob(defaultName),
   writeLogsToFile(false), archiveContent("")
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

GitBackupJob::GitBackupJob(const std::vector<std::pair<string, string> > &repositoryList)
 : AbstractBackupJob("Git Repository Backup"),
   writeLogsToFile(false), archiveContent("")
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

string GitBackupJob::GetTypeName() const
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
        const string &source,
        const string &destination,
        AbstractBackupJob::ResultCollection& results)
{
    if (FileTools::FolderExists(destination))
        UpdateGitRepository(destination, results);
    else
       RunGitClone(source, destination, results);
}

JobStatus* GitBackupJob::RestoreBackup(const string& source, const string& destination)
{
   ConsoleJob* gitCommand = new ConsoleJob("git", BuildCloneParameters(source, destination, false));
   JobStatus* status = gitCommand->Run();
   if (gitCommand->GetCommandReturnCode() == 128)
       status->SetDescription(invalidSourceRepositoryError);
   return status;
}

bool GitBackupJob::IsInvalidSourceError(const ConsoleJob &job) const
{
    if (!job.IsRunOk())
    {
        const string tagString = "does not appear to be a git repository";
        if (job.GetCommandOutput().find(tagString) != string::npos)
            return true;
    }

    return false;
}

bool GitBackupJob::AreSourcesConsistent() const
{
    vector<pair<string, string> >::const_iterator it=folderList.begin();
    for (; it!=folderList.end(); it++)
    {
        if (!FileTools::FolderExists(it->first))
            return false;
    }
    return true;
}

void GitBackupJob::UpdateGitRepository(const string &repository,
                                       AbstractBackupJob::ResultCollection &statusList)
{
    debugManager->AddDataLine<string>("Updating Git repository", repository);
    string originalDirectory = FileTools::GetCurrentFullPath();

    bool ok = GitCommonTools::ChangeCurrentDir(repository, statusList);
    if (!ok)
        return;

    // TODO : try again to use exceptions here
    const string oldHeadId = GetRepositoryHeadId();
    ok = FetchUpdates(repository, statusList);
    if (ok)
    {
        const string newHeadId = GetRepositoryHeadId();
        ComputeChanges(repository, oldHeadId, newHeadId, statusList);
    }

    GitCommonTools::ChangeCurrentDir(originalDirectory, statusList);
}

string GitBackupJob::GetRepositoryHeadId() const
{
    ConsoleJob command("git", "rev-parse HEAD");
    command.RunWithoutStatus();
    if (command.IsRunOk())
        return command.GetCommandOutput();
    else
        return string("");
}

bool GitBackupJob::FetchUpdates(const string &repository,
                                ResultCollection &statusList)
{
    ConsoleJob command("git", "remote update");
    command.RunWithoutStatus();
    if (!command.IsRunOk())
    {
        string errorMessage;
        if (IsInvalidSourceError(command))
            errorMessage = invalidSourceRepositoryError;
        else
            errorMessage = fetchUpdateError;

        JobStatus* status = new JobStatus(JobStatus::ERROR, invalidSourceRepositoryError);
        statusList.push_back(make_pair(status, new FileBackupReport()));
        AddToAttachedArchive(repository, command.GetCommandOutput());
    }

    return command.IsRunOk();
}

void GitBackupJob::ComputeChanges(
        const string& repository,
        const string &oldCommitId, const string &newCommitId,
        AbstractBackupJob::ResultCollection &statusList)
{
    string params = "diff --name-status ";
    params += oldCommitId + " " + newCommitId;
    ConsoleJob command("git", params);
    command.RunWithoutStatus();
    if (command.IsRunOk())
        CreateReport(repository, command.GetCommandOutput(), statusList);
    else
        throw command.GetCommandOutput();

}

void GitBackupJob::CreateReport(
        const std::string& repository,
        const std::string& commandOutput,
        AbstractBackupJob::ResultCollection &statusList)
{
    GitPlumbingReportParser parser;
    bool ok = parser.ParseBuffer(commandOutput);
    debugManager->AddDataLine<bool>("Parser result", ok);
    if (ok)
    {
        FileBackupReport* report = new FileBackupReport();
        parser.GetReport(*report);
        JobStatus* status = new JobStatus(JobStatus::OK, parser.GetMiniDescription());
        AddToAttachedArchive(repository, report->GetFullDescription());
        statusList.push_back(make_pair(status, report));
    }
    else
    {
        debugManager->AddDataLine<string>("Parser input", commandOutput);
        JobStatus* status = new JobStatus(JobStatus::OK_WITH_WARNINGS, reportCreationError);
        AddToAttachedArchive(repository, commandOutput);
        statusList.push_back(make_pair(status, static_cast<FileBackupReport*>(NULL)));
    }
}

void GitBackupJob::AddToAttachedArchive(
        const std::string& repository,
        const std::string& content)
{
    const string repositoryName = FileTools::GetFilenameFromUnixPath(repository);
    archiveContent += string("\nReport for ") + repositoryName + "\n";
    archiveContent += content;
}

void GitBackupJob::RunGitClone(const string &source,
                               const string &destination,
                               ResultCollection &statusList)
{
    debugManager->AddDataLine<string>("Git Clone on", repository);
    ConsoleJob* gitCommand = new ConsoleJob("git", BuildCloneParameters(source, destination, true));
    JobStatus* status = gitCommand->Run();
    debugManager->AddDataLine<int>("Clone result", gitCommand->GetCommandReturnCode());
    debugManager->AddDataLine<string>("Clone output", gitCommand->GetCommandOutput());
    if (gitCommand->GetCommandReturnCode() == 128)
        status->SetDescription(invalidSourceRepositoryError);
    else
    {
        status->SetDescription(repositoryCloneOk);
        AddToAttachedArchive(source, "Repository cloned");
    }

    statusList.push_back(make_pair(status, static_cast<FileBackupReport*>(NULL)));
    delete gitCommand;
}

string GitBackupJob::BuildCloneParameters(const string &source, const string &destination,
                                          const bool mirror) const
{
    string params("clone ");
    if (mirror)
       params += "--mirror ";

    if (isTargetLocal)
        params += source;
    else
        params += sshUser + "@" + sshHost + ":" + source;

    params += string(" 2>&1 ") + destination;
    return params;
}

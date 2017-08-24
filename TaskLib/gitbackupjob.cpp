#include "gitbackupjob.h"

#include <filetools.h>
#include <sstream>
#include <unistd.h>

#include "backupstatusmanager.h"
#include "consolejob.h"
#include "gitcommitreportparser.h"

using namespace std;

static const string invalidSourceRepositoryError        = "Invalid source repository";
static const string repositoryCloneOk                   = "Repository cloned successfully";
static const string invalidDestinationRepositoryError   = "Invalid destination repository";
static const string fetchUpdateError                    = "Update error";
static const string unknownError                        = "Unknown error";
static const string repositoryPullOk                    = "Repository successfully updated, see attached file.";
static const string reportCreationError                 = "Failed creating report";

static const string repositoryBackupMessage = "repositories backed up";

GitBackupJob::GitBackupJob()
 : AbstractBackupJob(), writeLogsToFile(false), archiveContent("")
{
    statusManager->SetItemBackupMessage(repositoryBackupMessage);
}

GitBackupJob::GitBackupJob(const GitBackupJob &other)
    : AbstractBackupJob(other),
      writeLogsToFile(other.writeLogsToFile),
      archiveContent(other.archiveContent)
{
    statusManager->SetItemBackupMessage(repositoryBackupMessage);
}

GitBackupJob::GitBackupJob(const std::vector<std::pair<string, string> > &repositoryList)
 : AbstractBackupJob(), writeLogsToFile(false), archiveContent("")
{
    folderList = repositoryList;
    statusManager->SetItemBackupMessage(repositoryBackupMessage);
}

GitBackupJob::~GitBackupJob()
{
}

string GitBackupJob::GetName()
{
    return "Git Repository Backup";
}

AbstractJob *GitBackupJob::Clone()
{
    return new GitBackupJob(*this);
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

JobStatus *GitBackupJob::CreateGlobalStatus(const AbstractBackupJob::ResultCollection &results)
{
    return AbstractBackupJob::CreateGlobalStatus(results);
}

string GitBackupJob::GetCorrectRepositoryWord() const
{
    return (folderList.size() == 1) ? "repository" : "repositories";
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

    chdir(repository.c_str());

    const string oldHeadId = GetRepositoryHeadId();
    bool ok = FetchUpdates(repository, statusList);
    if (ok)
    {
        const string newHeadId = GetRepositoryHeadId();
        ComputeChanges(repository, oldHeadId, newHeadId, statusList);
    }

    chdir(originalDirectory.c_str());
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
    GitCommitReportParser parser;
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
        statusList.push_back(make_pair(status, new FileBackupReport()));
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
                               ResultCollection &statusList) const
{
    debugManager->AddDataLine<string>("Git Clone on", repository);
    ConsoleJob* gitCommand = new ConsoleJob("git", BuildGitParameters(source, destination));
    const string gitLogFile = FileTools::GetFilenameFromUnixPath(destination) + ".txt";
    JobStatus* status = gitCommand->Run();
    debugManager->AddDataLine<int>("Clone result", gitCommand->GetCommandReturnCode());
    debugManager->AddDataLine<string>("Clone output", gitCommand->GetCommandOutput());
    if (gitCommand->GetCommandReturnCode() == 128)
        status->SetDescription(invalidSourceRepositoryError);
    else
    {
        status->SetDescription(repositoryCloneOk);
        status->AddFileBuffer(gitLogFile, gitCommand->GetCommandOutput());
    }

    statusList.push_back(make_pair(status, new FileBackupReport()));
    delete gitCommand;
}

string GitBackupJob::BuildGitParameters(const string &source, const string &destination) const
{
    string params("clone --mirror ");
    if (isTargetLocal)
        params += source;
    else
        params += sshUser + "@" + sshHost + ":" + source;

    params += string(" 2>&1 ") + destination;
    return params;
}

JobStatus *GitBackupJob::CreateSingleRepositoryStatus(const ResultEntry& entry) const
{
    return new JobStatus(*entry.first);
}

JobStatus *GitBackupJob::CreateMultiRepositoryStatus(const ResultCollection& results) const
{
    JobStatus* status = new JobStatus();
    stringstream descriptionStream;
    unsigned int faultyRepositories = CountFaultyRepositories(results);
    if (faultyRepositories == 0)
    {
        status->SetCode(JobStatus::OK);
        descriptionStream << folderList.size();
        descriptionStream << " Git " << GetCorrectRepositoryWord() << " successfully backed up.";
    }
    else if (faultyRepositories == folderList.size())
    {
        status->SetCode(JobStatus::ERROR);
        descriptionStream << folderList.size();
        descriptionStream << " Git " << GetCorrectRepositoryWord() << " failed to backup.";
    }
    else
    {
        status->SetCode(JobStatus::OK_WITH_WARNINGS);
        int okRepositories = folderList.size()-faultyRepositories;
        descriptionStream << okRepositories;
        descriptionStream << " Git " << GetCorrectRepositoryWord() << " backed up, ";
        descriptionStream << faultyRepositories << " failed.";
    }

    ResultCollection::const_iterator it=results.begin();
    for (; it!=results.end(); ++it)
        status->AddAllFilesFromStatus(it->first);

    status->SetDescription(descriptionStream.str());
    return status;
}

unsigned int GitBackupJob::CountFaultyRepositories(const ResultCollection& results) const
{
    unsigned int faulties = 0;
    ResultCollection::const_iterator it=results.begin();
    for(; it!=results.end(); ++it)
    {
        if (it->first->GetCode() == JobStatus::ERROR)
            ++faulties;
    }
    return faulties;
}

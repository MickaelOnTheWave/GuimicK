#include "gitbackupjob.h"

#include <sstream>
#include <filetools.h>
#include <unistd.h>

#include "consolejob.h"
#include "gitreportparser.h"

using namespace std;

static const string invalidSourceRepositoryError        = "Invalid source repository";
static const string repositoryCloneOk                   = "Repository cloned successfully";
static const string invalidDestinationRepositoryError   = "Invalid destination repository";
static const string unknownError                        = "Unknown error";
static const string repositoryPullOk                    = "Repository successfully updated, see attached file.";

GitBackupJob::GitBackupJob()
 : AbstractBackupJob(), writeLogsToFile(false)
{
}

GitBackupJob::GitBackupJob(const GitBackupJob &other)
    : AbstractBackupJob(other),
      writeLogsToFile(other.writeLogsToFile)
{

}

GitBackupJob::GitBackupJob(const std::vector<std::pair<string, string> > &repositoryList)
 : AbstractBackupJob(), writeLogsToFile(false)
{
    folderList = repositoryList;
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
        RunGitPull(destination, results);
    else
        RunGitClone(source, destination, results);
}

JobStatus *GitBackupJob::CreateGlobalStatus(const AbstractBackupJob::ResultCollection &results)
{
    if (folderList.size() == 1)
        return CreateSingleRepositoryStatus(results.front());
    else
        return CreateMultiRepositoryStatus(results);
}

string GitBackupJob::GetCorrectRepositoryWord() const
{
    return (folderList.size() == 1) ? "repository" : "repositories";
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

void GitBackupJob::RunGitPull(const string &repository,
                              ResultCollection &statusList) const
{
    debugManager->AddDataLine<string>("Git Pull on", repository);
    string originalDirectory = FileTools::GetCurrentFullPath();
    chdir(repository.c_str());

    ConsoleJob* gitCommand = new ConsoleJob("git", "pull");
    JobStatus* status = gitCommand->Run();
    FileBackupReport* report = new FileBackupReport();

    debugManager->AddDataLine<int>("result", gitCommand->GetCommandReturnCode());
    debugManager->AddDataLine<string>("output", gitCommand->GetCommandOutput());

    if (gitCommand->GetCommandReturnCode() == 128)
        status->SetDescription(invalidDestinationRepositoryError);
    else if (gitCommand->GetCommandReturnCode() != 0)
        status->SetDescription(unknownError);
    else
    {
        const string gitLogFile = FileTools::GetFilenameFromUnixPath(repository) + ".txt";
        GitReportParser parser;
        string reportContent = "";
        bool ok = parser.ParseBuffer(gitCommand->GetCommandOutput());
        debugManager->AddDataLine<bool>("Parser result", ok);
        if (ok)
        {
            parser.GetReport(*report);
            reportContent = parser.GetFullDescription();
            status->SetDescription(parser.GetMiniDescription());
        }
        else
        {
            reportContent = gitCommand->GetCommandOutput();
            status->SetDescription(repositoryPullOk);
        }

        status->AddFileBuffer(gitLogFile, reportContent);

        if (writeLogsToFile)
        {
            string fullPath = string(originalDirectory) + "/" + gitLogFile;
            FileTools::WriteBufferToFile(fullPath, reportContent);
        }
    }

    statusList.push_back(make_pair(status, report));
    delete gitCommand;

    chdir(originalDirectory.c_str());
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
    string params("clone ");
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

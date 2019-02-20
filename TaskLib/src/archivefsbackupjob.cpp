#include "archivefsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"
#include "sshconsolejob.h"

#include "tartool.h"
//#include "tartools.h"

using namespace std;

static const string cleaningError = "error cleaning destination";
static const string copyingError = "error copying archive";
static const string remoteCleaningError = "remote archive not cleaned";

ArchiveFsBackupJob::ArchiveFsBackupJob(const std::string& jobName,
                                       ArchiveTool* _archiveTool)
    : AbstractBackupJob(jobName),
      localDestination(""),
      archiveTool(_archiveTool)
{
}

ArchiveFsBackupJob::ArchiveFsBackupJob(const ArchiveFsBackupJob& other)
    : AbstractBackupJob(other),
      localDestination(other.localDestination),
      archiveTool(other.archiveTool->Clone())
{
}

ArchiveFsBackupJob::~ArchiveFsBackupJob()
{
   delete archiveTool;
}

bool ArchiveFsBackupJob::Restore(const string &backupFile, const string &destination)
{
    if (FileTools::FolderExists(destination) == false)
    {
        bool result = FileTools::CreateFolder(destination);
        if (!result)
            return false;
    }

    const string params = string("-xf ") + backupFile + " -C " + destination;
    ConsoleJob commandJob("tar", params);
    commandJob.RunWithoutStatus();

    return (commandJob.GetCommandReturnCode() == 0);
}

string ArchiveFsBackupJob::GetLocalDestination() const
{
   return localDestination;
}

void ArchiveFsBackupJob::SetLocalDestination(const string &value)
{
    localDestination = value;
}

void ArchiveFsBackupJob::RunRepositoryBackup(const std::string &source,
                                                const std::string &destination,
                                                AbstractBackupJob::ResultCollection &results)
{
   if (target.isLocal)
      UpdateBackupArchive(source, destination, results);
   else
   {
      // TODO : do it
   }
}

JobStatus* ArchiveFsBackupJob::RestoreBackupFromServer(const string& source,
                                                const string& destination)
{
   bool ok = Restore(source, destination);
   return new JobStatus(ok ? JobStatus::Ok : JobStatus::Error);
}

void AddResultToCollection(const ArchiveToolResult& result,
                           AbstractBackupJob::ResultCollection &results)
{
   JobStatus* jobStatus = new JobStatus();
   jobStatus->SetCode(result.isOk ? JobStatus::Ok : JobStatus::Error);
   if (result.errorMessage != "")
      jobStatus->SetDescription(result.errorMessage);

   FileBackupReport* backupReport = new FileBackupReport();
   backupReport->AddAsAdded(result.FileList);
   results.push_back(make_pair(jobStatus, backupReport));
}

// TODO : maybe there is a better architectural option to these result collections :
// create a JobBackupStatus that stores backup report.
bool ArchiveFsBackupJob::UpdateBackupArchive(const string &folderToBackup,
                                                const string &archiveName,
                                                AbstractBackupJob::ResultCollection &results)
{
   archiveTool->Initialize(archiveName);
   ArchiveToolResult result;
   archiveTool->AddToArchive(folderToBackup, result);
   AddResultToCollection(result, results);
   return result.isOk;
}

bool ArchiveFsBackupJob::RemovePreviousArchive(const string &destination,
                                               AbstractBackupJob::ResultCollection &results)
{
    if (FileTools::FileExists(destination))
        FileTools::RemoveFile(destination);

    const bool ok = !FileTools::FileExists(destination);
    if (!ok)
        AddStatusToResults(results, JobStatus::Error, cleaningError);
    return ok;
}

bool ArchiveFsBackupJob::CopyBackupArchiveToDestination(
        const string &destination,
        AbstractBackupJob::ResultCollection &results)
{
    stringstream params;
    params << target.sshUser << "@" << target.sshHost << ":";
    params << localDestination << " " << destination;
    ConsoleJob scpCommand("scp", params.str());
    scpCommand.RunWithoutStatus();

    const bool isRunOk = scpCommand.IsRunOk();
    if (!isRunOk)
        AddStatusToResults(results, JobStatus::Error, copyingError);

    return isRunOk;
}

bool ArchiveFsBackupJob::CleanBackupArchiveFromSource(AbstractBackupJob::ResultCollection &results)
{
    string parameters = string("-f ") + localDestination;
    SshConsoleJob* remoteJob = new SshConsoleJob(new ConsoleJob("rm", parameters));
    remoteJob->SetTarget(target.sshUser, target.sshHost);

    JobStatus* unusedStatus = remoteJob->Run();
    delete unusedStatus;

    const bool isOk = (remoteJob->GetCommandReturnCode() == 0);
    if (!isOk)
        AddStatusToResults(results, JobStatus::OkWithWarnings, remoteCleaningError);

    delete remoteJob;
    return isOk;
}

void ArchiveFsBackupJob::AddStatusToResults(AbstractBackupJob::ResultCollection &results,
                                               const int code, const string &message)
{
    JobStatus* status = new JobStatus(code, message);
    results.push_back(pair<JobStatus*, FileBackupReport*>(status, NULL));
}

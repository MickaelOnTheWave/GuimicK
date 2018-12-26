#include "zipandcopyfsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"
#include "sshconsolejob.h"

#include "tartool.h"
//#include "tartools.h"

using namespace std;

static const string cleaningError = "error cleaning destination";
static const string copyingError = "error copying archive";
static const string remoteCleaningError = "remote archive not cleaned";
static const string defaultName = "ZipAndCopy Backup";

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob()
    : AbstractBackupJob(defaultName),
      localDestination("")
{
}

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob(const ZipAndCopyFsBackupJob& other)
    : AbstractBackupJob(other),
      localDestination(other.localDestination)
{
}

AbstractJob *ZipAndCopyFsBackupJob::Clone()
{
   return new ZipAndCopyFsBackupJob(*this);
}

string ZipAndCopyFsBackupJob::GetTypeName() const
{
   return defaultName;
}

bool ZipAndCopyFsBackupJob::Restore(const string &backupFile, const string &destination)
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

string ZipAndCopyFsBackupJob::GetLocalDestination() const
{
   return localDestination;
}

void ZipAndCopyFsBackupJob::SetLocalDestination(const string &value)
{
    localDestination = value;
}

void ZipAndCopyFsBackupJob::RunRepositoryBackup(const std::string &source,
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

JobStatus* ZipAndCopyFsBackupJob::RestoreBackupFromServer(const string& source,
                                                const string& destination)
{
   bool ok = Restore(source, destination);
   return new JobStatus(ok ? JobStatus::OK : JobStatus::ERROR);
}

void AddResultToCollection(const ArchiveToolResult& result,
                           AbstractBackupJob::ResultCollection &results)
{
   JobStatus* jobStatus = new JobStatus();
   jobStatus->SetCode(result.isOk ? JobStatus::OK : JobStatus::ERROR);

   FileBackupReport* backupReport = new FileBackupReport();
   backupReport->AddAsAdded(result.FileList);
   results.push_back(make_pair(jobStatus, backupReport));
}

// TODO : maybe there is a better architectural option to these result collections :
// create a JobBackupStatus that stores backup report.
bool ZipAndCopyFsBackupJob::UpdateBackupArchive(const string &folderToBackup,
                                                const string &archiveName,
                                                AbstractBackupJob::ResultCollection &results)
{
   ArchiveTool* archiveTool = CreateArchiveTool(archiveName);
   ArchiveToolResult result;
   archiveTool->AddToArchive(folderToBackup, result);
   AddResultToCollection(result, results);
   delete archiveTool;
   return result.isOk;
}

bool ZipAndCopyFsBackupJob::RemovePreviousArchive(const string &destination,
                                               AbstractBackupJob::ResultCollection &results)
{
    if (FileTools::FileExists(destination))
        FileTools::RemoveFile(destination);

    const bool ok = !FileTools::FileExists(destination);
    if (!ok)
        AddStatusToResults(results, JobStatus::ERROR, cleaningError);
    return ok;
}

bool ZipAndCopyFsBackupJob::CopyBackupArchiveToDestination(
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
        AddStatusToResults(results, JobStatus::ERROR, copyingError);

    return isRunOk;
}

bool ZipAndCopyFsBackupJob::CleanBackupArchiveFromSource(AbstractBackupJob::ResultCollection &results)
{
    string parameters = string("-f ") + localDestination;
    SshConsoleJob* remoteJob = new SshConsoleJob(new ConsoleJob("rm", parameters));
    remoteJob->SetTarget(target.sshUser, target.sshHost);

    JobStatus* unusedStatus = remoteJob->Run();
    delete unusedStatus;

    const bool isOk = (remoteJob->GetCommandReturnCode() == 0);
    if (!isOk)
        AddStatusToResults(results, JobStatus::OK_WITH_WARNINGS, remoteCleaningError);

    delete remoteJob;
    return isOk;
}

void ZipAndCopyFsBackupJob::AddStatusToResults(AbstractBackupJob::ResultCollection &results,
                                               const int code, const string &message)
{
    JobStatus* status = new JobStatus(code, message);
    results.push_back(pair<JobStatus*, FileBackupReport*>(status, NULL));
}

ArchiveTool* ZipAndCopyFsBackupJob::CreateArchiveTool(const string& filename) const
{
   TarTool* tool = new TarTool(filename);
   tool->SetGzipCompression(false);
   return tool;
}

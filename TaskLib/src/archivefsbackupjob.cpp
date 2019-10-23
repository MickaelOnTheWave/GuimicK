#include "archivefsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"
#include "sshconsolejob.h"

#include "tartool.h"
//#include "tartools.h"

using namespace std;

static const wstring cleaningError = L"error cleaning destination";
static const wstring copyingError = L"error copying archive";
static const wstring remoteCleaningError = L"remote archive not cleaned";

ArchiveFsBackupJob::ArchiveFsBackupJob(const wstring& jobName,
                                       ArchiveTool* _archiveTool)
    : AbstractBackupJob(jobName),
      archiveTool(_archiveTool),
      localDestination(L"")
{
}

ArchiveFsBackupJob::ArchiveFsBackupJob(const ArchiveFsBackupJob& other)
    : AbstractBackupJob(other),
      archiveTool(other.archiveTool->Clone()),
      localDestination(other.localDestination)
{
}

ArchiveFsBackupJob::~ArchiveFsBackupJob()
{
   delete archiveTool;
}

bool ArchiveFsBackupJob::Restore(const wstring &backupFile, const wstring &destination)
{
    if (FileTools::FolderExists(destination) == false)
    {
        bool result = FileTools::CreateFolder(destination);
        if (!result)
            return false;
    }

    const wstring params = wstring(L"-xf ") + backupFile + L" -C " + destination;
    ConsoleJob commandJob(L"tar", params);
    commandJob.RunWithoutStatus();

    return (commandJob.GetCommandReturnCode() == 0);
}

wstring ArchiveFsBackupJob::GetLocalDestination() const
{
   return localDestination;
}

void ArchiveFsBackupJob::SetLocalDestination(const wstring &value)
{
    localDestination = value;
}

void ArchiveFsBackupJob::RunRepositoryBackup(const std::wstring &source,
                                                const std::wstring &destination,
                                                AbstractBackupJob::ResultCollection &results)
{
   if (target.isLocal)
      UpdateBackupArchive(source, destination, results);
   else
   {
      // TODO : do it
   }
}

JobStatus* ArchiveFsBackupJob::RestoreBackupFromServer(const wstring& source,
                                                const wstring& destination)
{
   bool ok = Restore(source, destination);
   return new JobStatus(ok ? JobStatus::Ok : JobStatus::Error);
}

void AddResultToCollection(const ArchiveToolResult& result,
                           AbstractBackupJob::ResultCollection &results)
{
   JobStatus* jobStatus = new JobStatus();
   jobStatus->SetCode(result.isOk ? JobStatus::Ok : JobStatus::Error);
   if (result.errorMessage != L"")
      jobStatus->SetDescription(result.errorMessage);

   results.push_back(make_pair(jobStatus, new FileBackupReport(result.FileList)));
}

// TODO : maybe there is a better architectural option to these result collections :
// create a JobBackupStatus that stores backup report.
bool ArchiveFsBackupJob::UpdateBackupArchive(const wstring &folderToBackup,
                                                const wstring &archiveName,
                                                AbstractBackupJob::ResultCollection &results)
{
   archiveTool->Initialize(archiveName);
   ArchiveToolResult result;
   archiveTool->AddToArchive(folderToBackup, result);
   AddResultToCollection(result, results);
   return result.isOk;
}

bool ArchiveFsBackupJob::RemovePreviousArchive(const wstring &destination,
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
        const wstring &destination,
        AbstractBackupJob::ResultCollection &results)
{
    wstringstream params;
    params << target.sshUser << "@" << target.sshHost << ":";
    params << localDestination << " " << destination;
    ConsoleJob scpCommand(L"scp", params.str());
    scpCommand.RunWithoutStatus();

    const bool isRunOk = scpCommand.IsRunOk();
    if (!isRunOk)
        AddStatusToResults(results, JobStatus::Error, copyingError);

    return isRunOk;
}

bool ArchiveFsBackupJob::CleanBackupArchiveFromSource(AbstractBackupJob::ResultCollection &results)
{
    wstring parameters = wstring(L"-f ") + localDestination;
    SshConsoleJob* remoteJob = new SshConsoleJob(new ConsoleJob(L"rm", parameters));
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
                                               const int code, const wstring &message)
{
    JobStatus* status = new JobStatus(code, message);
    results.push_back(pair<JobStatus*, FileBackupReport*>(status, NULL));
}

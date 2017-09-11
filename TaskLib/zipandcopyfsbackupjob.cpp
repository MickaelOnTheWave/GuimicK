#include "zipandcopyfsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"
#include "sshconsolejob.h"
#include "tarcommandparser.h"

using namespace std;

static const string reportCreationError = "Failed creating report";
static const string tarCommandError = "tar command failed";
static const string cleaningError = "error cleaning destination";
static const string copyingError = "error copying archive";
static const string remoteCleaningError = "remote archive not cleaned";

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob()
    : AbstractBackupJob(), localDestination("")
{
}

ZipAndCopyFsBackupJob::ZipAndCopyFsBackupJob(const ZipAndCopyFsBackupJob& other)
    : AbstractBackupJob(other),
      localDestination(other.localDestination)
{
}

std::string ZipAndCopyFsBackupJob::GetName()
{
    return string("ZipAndCopy Backup");
}

AbstractJob *ZipAndCopyFsBackupJob::Clone()
{
    return new ZipAndCopyFsBackupJob(*this);
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

void ZipAndCopyFsBackupJob::SetLocalDestination(const string &value)
{
    localDestination = value;
}

void ZipAndCopyFsBackupJob::RunRepositoryBackup(const std::string &source,
                                                const std::string &destination,
                                                AbstractBackupJob::ResultCollection &results)
{
    bool ok = RemovePreviousArchive(destination, results);

    const string localArchive = (isTargetLocal) ? destination : localDestination;

    if (ok)
        ok = CreateBackupArchive(source, localArchive, results);

    if (!isTargetLocal)
    {
        if (ok)
            ok = CopyBackupArchiveToDestination(destination, results);

        if (ok)
            CleanBackupArchiveFromSource(results);
    }
}

// TODO : maybe there is a better architectural option to these result collections :
// create a JobBackupStatus that stores backup report.
bool ZipAndCopyFsBackupJob::CreateBackupArchive(const string &folderToBackup,
                                                const string &archiveName,
                                                AbstractBackupJob::ResultCollection &results)
{
    JobStatus* status = new JobStatus();

    const string params = string("-cpzvf ") + archiveName + " -C " + folderToBackup + " .";
    AbstractConsoleJob* commandJob = CreateBackupConsoleJob(params);
    commandJob->SetParentDebugManager(debugManager);

    JobStatus* unusedStatus = commandJob->Run();
    delete unusedStatus;

    bool returnValue = false;
    if (commandJob->IsRunOk())
    {
        TarCommandParser parser(commandJob->GetCommand());
        bool ok = parser.ParseBuffer(commandJob->GetCommandOutput());
        if (ok)
        {
            FileBackupReport* report = new FileBackupReport();
            parser.GetReport(*report);

            status->SetCode(JobStatus::OK);
            results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, report));
        }
        else
        {
            status->SetCode(JobStatus::OK_WITH_WARNINGS);
            status->SetDescription(reportCreationError);
            results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, NULL));
        }
        returnValue = true;
    }
    else
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription(tarCommandError);
        debugManager->AddDataLine<string>("tar output", commandJob->GetCommandOutput());
        debugManager->AddDataLine<string>("tar code", commandJob->GetCommandReturnCode());
        results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, NULL));
        returnValue = false;
    }

    delete commandJob;
    return returnValue;
}

bool ZipAndCopyFsBackupJob::RemovePreviousArchive(const string &destination,
                                               AbstractBackupJob::ResultCollection &results)
{
    if (FileTools::FileExists(destination))
    {
        // TODO : implement this RemoveFile
        //FileTools::RemoveFile(finalBackupArchive);
        const string params = string("-Rf ") + destination;
        ConsoleJob::Run("rm", params);
    }

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
    params << sshUser << "@" << sshHost << ":" << localDestination << " " << destination;
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
    SshConsoleJob* remoteJob = new SshConsoleJob("", new ConsoleJob("rm", parameters));
    remoteJob->SetTarget(sshUser, sshHost);

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
    results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, NULL));
}

AbstractConsoleJob *ZipAndCopyFsBackupJob::CreateBackupConsoleJob(const string &parameters)
{
    ConsoleJob* job = new ConsoleJob("tar", parameters);
    if (isTargetLocal)
        return job;
    else
    {
        SshConsoleJob* remoteJob = new SshConsoleJob("", job);
        remoteJob->SetTarget(sshUser, sshHost);
        return remoteJob;
    }
}

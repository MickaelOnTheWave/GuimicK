#include "abstractcopyfsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"
#include "tools.h"

using namespace std;

static const string attachmentName = "CopyFsBackupJob.txt";

static const string errorReportCreation = "Error creating report";
static const string errorCopyCommand = "Copy command failed";

AbstractCopyFsBackupJob::AbstractCopyFsBackupJob(const string &debugFilename)
    : AbstractBackupJob(debugFilename)
{
    statusManager.SetDebugManager(&debugManager);
}

AbstractCopyFsBackupJob::AbstractCopyFsBackupJob(const AbstractCopyFsBackupJob &other)
    : AbstractBackupJob(other), statusManager(other.statusManager)
{
    statusManager.SetDebugManager(&debugManager);
}

AbstractCopyFsBackupJob::~AbstractCopyFsBackupJob()
{
}

int AbstractCopyFsBackupJob::RunOnParameters(const string &source, const string &destination)
{
    ConsoleJob commandJob;
    PrepareCopyCommand(source, destination, commandJob);

    commandJob.RunWithoutStatus();

    debugManager.AddDataLine<string>("Copy command", commandJob.GetCommand() + " " + commandJob.GetCommandParameters());
    debugManager.AddDataLine<string>("Copy output", commandJob.GetCommandOutput());
    debugManager.AddDataLine<int>("Copy value", commandJob.GetCommandReturnCode());

    return commandJob.GetCommandReturnCode();
}

void AbstractCopyFsBackupJob::SetJoinAllBackups(const bool value)
{
    statusManager.SetJoinReports(value);
}

void AbstractCopyFsBackupJob::RunRepositoryBackup(const string &source,
                                          const string &destination,
                                          ResultCollection &results)
{
    if (FileTools::FolderExists(destination) == false)
        FileTools::CreateFolder(destination);

    RunCopy(source, destination, results);
}

JobStatus *AbstractCopyFsBackupJob::CreateGlobalStatus(const AbstractBackupJob::ResultCollection &results)
{
    return statusManager.CreateGlobalStatus(results, folderList);
}

void AbstractCopyFsBackupJob::RunCopy(const string &source, const string &destination,
                                 AbstractBackupJob::ResultCollection &results)
{
    ConsoleJob copyCommand;
    PrepareCopyCommand(source, destination, copyCommand);
    JobStatus* status = copyCommand.Run();

    if (status->GetCode() == JobStatus::OK)
        CreateReport(destination, copyCommand.GetCommandOutput(), results);
    else
        CreateCopyErrorReport(copyCommand.GetCommandOutput(), results);
}

void AbstractCopyFsBackupJob::CreateCopyErrorReport(const std::string& message, AbstractBackupJob::ResultCollection &results)
{
    JobStatus* status = new JobStatus(JobStatus::ERROR, errorCopyCommand);
    status->AddFileBuffer(attachmentName, message);
    results.push_back(make_pair(status, new FileBackupReport()));
}

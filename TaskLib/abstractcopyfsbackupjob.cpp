#include "abstractcopyfsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"
#include "tools.h"

using namespace std;

static const string errorReportCreation = "Error creating report";
static const string errorCopyCommand = "Copy command failed";

AbstractCopyFsBackupJob::AbstractCopyFsBackupJob(const string& _title)
   : AbstractBackupJob(_title)
{
}

AbstractCopyFsBackupJob::AbstractCopyFsBackupJob(const AbstractCopyFsBackupJob &other)
    : AbstractBackupJob(other)
{
}

AbstractCopyFsBackupJob::~AbstractCopyFsBackupJob()
{
}

int AbstractCopyFsBackupJob::RunOnParameters(const string &source, const string &destination)
{
    ConsoleJob commandJob;
    PrepareCopyCommand(source, destination, commandJob);

    commandJob.RunWithoutStatus();

    debugManager->AddDataLine<string>("Copy command", commandJob.GetCommand() + " " + commandJob.GetCommandParameters());
    debugManager->AddDataLine<string>("Copy output", commandJob.GetCommandOutput());
    debugManager->AddDataLine<int>("Copy value", commandJob.GetCommandReturnCode());

    return commandJob.GetCommandReturnCode();
}

void AbstractCopyFsBackupJob::RunRepositoryBackup(const string &source,
                                          const string &destination,
                                          ResultCollection &results)
{
    if (FileTools::FolderExists(destination) == false)
        FileTools::CreateFolder(destination);

    RunCopy(source, destination, results);
}

JobStatus* AbstractCopyFsBackupJob::RestoreBackup(const string& source, const string& destination)
{
   ConsoleJob copyCommand;
   PrepareCopyCommand(source, destination, copyCommand);
   return copyCommand.Run();
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
    status->AddFileBuffer(GetAttachmentName(), message);
    results.push_back(make_pair(status, new FileBackupReport()));
}

#include "abstractcopyfsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"
#include "tools.h"

using namespace std;

static const wstring errorReportCreation = L"Error creating report";
static const wstring errorCopyCommand = L"Copy command failed";

AbstractCopyFsBackupJob::AbstractCopyFsBackupJob(const wstring& _title)
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

int AbstractCopyFsBackupJob::RunOnParameters(const wstring &source, const wstring &destination)
{
    ConsoleJob commandJob;
    PrepareCopyCommand(source, destination, commandJob);

    commandJob.RunWithoutStatus();
    AddCommandDebugInformation(commandJob);

    return commandJob.GetCommandReturnCode();
}

void AbstractCopyFsBackupJob::RunRepositoryBackup(const wstring &source,
                                                  const wstring &destination,
                                                  ResultCollection &results)
{
   const wstring copyDestination = repository + destination;
   if (FileTools::FolderExists(copyDestination) == false)
     FileTools::CreateFolder(copyDestination);

   RunCopy(source, copyDestination, results);
}

JobStatus* AbstractCopyFsBackupJob::RestoreBackupFromServer(const wstring& source, const wstring& destination)
{
   ConsoleJob copyCommand;
   PrepareCopyCommand(source, destination, copyCommand);
   return copyCommand.Run();
}

void AbstractCopyFsBackupJob::RunCopy(const wstring &source, const wstring &destination,
                                 AbstractBackupJob::ResultCollection &results)
{
    ConsoleJob copyCommand;
    PrepareCopyCommand(source, destination, copyCommand);
    JobStatus* status = copyCommand.Run();
    AddCommandDebugInformation(copyCommand);

    if (status->GetCode() == JobStatus::Ok)
        CreateReport(destination, copyCommand.GetCommandOutput(), results);
    else
        CreateCopyErrorReport(copyCommand.GetCommandOutput(), results);
}

void AbstractCopyFsBackupJob::CreateCopyErrorReport(const std::wstring& message, AbstractBackupJob::ResultCollection &results)
{
    JobStatus* status = new JobStatus(JobStatus::Error, errorCopyCommand);
    status->AddFileBuffer(GetAttachmentName(), message);
    results.push_back(make_pair(status, new FileBackupReport()));
}

void AbstractCopyFsBackupJob::AddCommandDebugInformation(const ConsoleJob& job)
{
   const wstring fullCommand = job.GetCommand() + L" " + job.GetCommandParameters();
   debugManager->AddDataLine<wstring>(L"Copy command", fullCommand);
   debugManager->AddDataLine<wstring>(L"Copy output", job.GetCommandOutput());
   debugManager->AddDataLine<int>(L"Copy value", job.GetCommandReturnCode());
}

#include "copyfsbackupjob.h"

#include "consolejob.h"
#include "filetools.h"
#include "tools.h"

using namespace std;

static const string attachmentName = "CopyFsBackupJob.txt";

static const string errorReportCreation = "Error creating report";
static const string errorCopyCommand = "Copy command failed";

CopyFsBackupJob::CopyFsBackupJob() : AbstractBackupJob("CopyFsBackup")
{
    statusManager.SetDebugManager(&debugManager);
}

CopyFsBackupJob::CopyFsBackupJob(const CopyFsBackupJob &other)
    : AbstractBackupJob(other), statusManager(other.statusManager)
{
    statusManager.SetDebugManager(&debugManager);
}

std::string CopyFsBackupJob::GetName()
{
    return string("CopyFsBackup");
}

AbstractJob *CopyFsBackupJob::Clone()
{
    return new CopyFsBackupJob(*this);
}

int CopyFsBackupJob::RunOnParameters(const string &source, const string &destination)
{
    ConsoleJob commandJob;
    PrepareCopyCommand(source, destination, commandJob);

    commandJob.RunWithoutStatus();

    debugManager.AddStringDataLine("Copy command", commandJob.GetCommand() + " " + commandJob.GetCommandParameters());
    debugManager.AddStringDataLine("Copy output", commandJob.GetCommandOutput());
    debugManager.AddIntDataLine("Copy value", commandJob.GetCommandReturnCode());

    return commandJob.GetCommandReturnCode();
}

void CopyFsBackupJob::SetJoinAllBackups(const bool value)
{
    statusManager.SetJoinReports(value);
}

void CopyFsBackupJob::RunRepositoryBackup(const string &source,
                                          const string &destination,
                                          ResultCollection &results)
{
    if (FileTools::FolderExists(destination) == false)
        FileTools::CreateFolder(destination);

    ConsoleJob copyCommand;
    PrepareCopyCommand(source, destination, copyCommand);
    JobStatus* status = copyCommand.Run();

    if (status->GetCode() == JobStatus::OK)
        CreateReport(destination, results);
    else
        CreateCopyErrorReport(copyCommand.GetCommandOutput(), results);
}

JobStatus *CopyFsBackupJob::CreateGlobalStatus(const AbstractBackupJob::ResultCollection &results)
{
    return statusManager.CreateGlobalStatus(results, folderList);
}

void CopyFsBackupJob::PrepareCopyCommand(const string &source, const string &destination, ConsoleJob &commandJob)
{
    const string command = (isTargetLocal ? "cp" : "scp");
    string params;
    if (isTargetLocal)
        params = "-R ";
    else
        params = string("-r ") + sshUser + "@" + sshHost + ":";
    params += source + "* " + destination;

    commandJob.SetCommand(command);
    commandJob.SetCommandParameters(params);
}

void CopyFsBackupJob::CreateReport(const string &destination, AbstractBackupJob::ResultCollection &results)
{
    FileBackupReport* report = new FileBackupReport();
    JobStatus* status = new JobStatus();

    ConsoleJob lsCommand("ls", destination);
    lsCommand.RunWithoutStatus();
    if (lsCommand.GetCommandReturnCode() == 0)
    {
        vector<string> fileList;
        Tools::TokenizeString(lsCommand.GetCommandOutput(), '\n', fileList);

        report->AddAsAdded(fileList);
        status->SetCode(JobStatus::OK);
        status->SetDescription(report->GetMiniDescription());
        status->AddFileBuffer(attachmentName, report->GetFullDescription());
    }
    else
    {
        status->SetCode(JobStatus::OK_WITH_WARNINGS);
        status->SetDescription(errorReportCreation);
        status->AddFileBuffer(attachmentName, lsCommand.GetCommandOutput());
    }

    results.push_back(make_pair(status, report));
}

void CopyFsBackupJob::CreateCopyErrorReport(const std::string& message, AbstractBackupJob::ResultCollection &results)
{
    JobStatus* status = new JobStatus(JobStatus::ERROR, errorCopyCommand);
    status->AddFileBuffer(attachmentName, message);
    results.push_back(make_pair(status, new FileBackupReport()));
}

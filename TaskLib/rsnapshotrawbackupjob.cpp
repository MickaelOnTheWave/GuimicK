#include "rsnapshotrawbackupjob.h"

#include <sstream>
#include <unistd.h>

#include "rsnapshoterroranalyzer.h"

using namespace std;

RsnapshotRawBackupJob::RsnapshotRawBackupJob(const std::string &_backupRepositoryPath,
                                             const std::string &_rsnapshotConfFile)
    : AbstractJob(),
      configurationFile(_rsnapshotConfFile),
      repository(_backupRepositoryPath), waitBeforeRun(false),
      reportJobOutput("")
{
}

RsnapshotRawBackupJob::RsnapshotRawBackupJob(const RsnapshotRawBackupJob &other)
    : AbstractJob(),
      configurationFile(other.configurationFile),
      repository(other.repository), waitBeforeRun(other.waitBeforeRun),
      reportJobOutput(other.reportJobOutput)
{
}

RsnapshotRawBackupJob::~RsnapshotRawBackupJob()
{
}

std::string RsnapshotRawBackupJob::GetName()
{
    return string("Rsnapshot Backup");
}

AbstractJob *RsnapshotRawBackupJob::Clone()
{
    return new RsnapshotRawBackupJob(*this);
}

bool RsnapshotRawBackupJob::InitializeFromClient(Client *)
{
    return true;
}

bool RsnapshotRawBackupJob::IsInitialized()
{
    bool isRepositoryOk = (repository != "");
    bool isConfigurationOk = (configurationFile != "");
    return isRepositoryOk && isConfigurationOk;
}

JobStatus *RsnapshotRawBackupJob::Run()
{
    if (waitBeforeRun)
        sleep(1);

    JobStatus* backupStatus = RunBackup();
    if (backupStatus->GetCode() != JobStatus::OK)
        return backupStatus;
    else
        delete backupStatus;

    JobStatus* reportStatus = RunReportCreation();
    if (reportStatus->GetCode() != JobStatus::OK)
        return reportStatus;
    else
        delete reportStatus;

    return CreateParsedReportStatus();
}

void RsnapshotRawBackupJob::SetRepository(const string &value)
{
    repository = value;
}

void RsnapshotRawBackupJob::SetConfigurationFile(const string &value)
{
    configurationFile = value;
}

void RsnapshotRawBackupJob::SetWaitBeforeRun(const bool value)
{
    waitBeforeRun = value;
}

JobStatus *RsnapshotRawBackupJob::RunBackup()
{
    ConsoleJob* backupJob = CreateBackupCommandJob();
    JobStatus* backupStatus = backupJob->Run();
    debugManager->AddDataLine<string>("Full command output", backupJob->GetCommandOutput());
    if (backupStatus->GetCode() != JobStatus::OK)
    {
        if (backupJob->GetCommandReturnCode() == 1)
        {
            RsnapshotErrorAnalyzer analyzer(backupJob->GetCommandOutput());
            if (analyzer.IsOutOfSpaceError())
                backupStatus->SetDescription("Not enough disk space to perform backup");
            else if (analyzer.IsInvalidFolderError())
                backupStatus->SetDescription("Tried to backup invalid folder");
            else
            {
                backupStatus->SetDescription("Fatal error. See attached file.");
                backupStatus->AddFileBuffer(GetAttachmentName(), backupJob->GetCommandOutput());
            }
        }
        else if (backupJob->GetCommandReturnCode() == 2)
        {
            backupStatus->SetDescription("RSnapshot executed with some warnings. See attached file.");
            backupStatus->SetCode(JobStatus::OK_WITH_WARNINGS);
            backupStatus->AddFileBuffer(GetAttachmentName(), backupJob->GetCommandOutput());
        }
        else
        {
            string description("Unknown error running rsnapshot. Return code : ");
            stringstream stream;
            stream << backupJob->GetCommandReturnCode();
            description += stream.str();
            backupStatus->SetDescription(description);
            backupStatus->AddFileBuffer(GetAttachmentName(), backupJob->GetCommandOutput());
        }
    }

    delete backupJob;
    return debugManager->UpdateStatus(backupStatus);
}

JobStatus *RsnapshotRawBackupJob::RunReportCreation()
{
    ConsoleJob* reportJob = CreateReportCommandJob();
    JobStatus* reportStatus = reportJob->Run();
    debugManager->AddDataLine<string>("Full report output", reportJob->GetCommandOutput());
    if (reportStatus->GetCode() == JobStatus::OK)
        reportJobOutput = reportJob->GetCommandOutput();
    else
    {
        string description("Error creating rsnapshot report. Return code : ");
        stringstream stream;
        stream << reportJob->GetCommandReturnCode();
        description += stream.str();
        reportStatus->SetDescription(description);
        reportStatus->SetCode(JobStatus::OK_WITH_WARNINGS);
        reportStatus->AddFileBuffer(GetAttachmentName(), reportJob->GetCommandOutput());
    }

    delete reportJob;
    return debugManager->UpdateStatus(reportStatus);
}

JobStatus *RsnapshotRawBackupJob::CreateParsedReportStatus()
{
    RSnapshotReportParser parser;
    parser.ParseBuffer(reportJobOutput);

    JobStatus* status = new JobStatus(JobStatus::OK, parser.GetMiniDescription());
    status->AddFileBuffer(GetAttachmentName(), parser.GetFullDescription());
    return debugManager->UpdateStatus(status);
}

ConsoleJob *RsnapshotRawBackupJob::CreateBackupCommandJob()
{
    const string parameters = string("-c ") + configurationFile + " weekly";
    return new ConsoleJob("rsnapshot", parameters);
}

ConsoleJob *RsnapshotRawBackupJob::CreateReportCommandJob()
{
    const string parameters = string("-v ") + repository + "/weekly.0 " + repository + "/weekly.1 2>&1";
    return new ConsoleJob("rsnapshot-diff", parameters);
}

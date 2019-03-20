#include "rsnapshotrawbackupjob.h"

#include <sstream>

#include "filetools.h"
#include "rsnapshoterroranalyzer.h"
#include "tools.h"

using namespace std;

RsnapshotRawBackupJob::RsnapshotRawBackupJob(const wstring &_backupRepositoryPath,
                                             const wstring &_rsnapshotConfFile)
    : AbstractJob(L"Rsnapshot Backup"),
      configurationFile(_rsnapshotConfFile),
      repository(_backupRepositoryPath), waitBeforeRun(false),
      reportJobOutput(L"")
{
}

RsnapshotRawBackupJob::RsnapshotRawBackupJob(const RsnapshotRawBackupJob &other)
    : AbstractJob(other),
      configurationFile(other.configurationFile),
      repository(other.repository), waitBeforeRun(other.waitBeforeRun),
      reportJobOutput(other.reportJobOutput)
{
}

RsnapshotRawBackupJob::~RsnapshotRawBackupJob()
{
}

AbstractJob *RsnapshotRawBackupJob::Clone()
{
    return new RsnapshotRawBackupJob(*this);
}

bool RsnapshotRawBackupJob::IsInitialized()
{
    const bool isRepositoryOk = (repository != L"");
    const bool isConfigurationOk = (configurationFile != L"");
    return isRepositoryOk && isConfigurationOk;
}

JobStatus *RsnapshotRawBackupJob::Run()
{
    if (waitBeforeRun)
        Tools::Wait(1);

    JobStatus* backupStatus = RunBackup();
    if (backupStatus->GetCode() != JobStatus::Ok)
        return backupStatus;
    else
        delete backupStatus;

    return RunReportCreation();
}

wstring RsnapshotRawBackupJob::GetRepository() const
{
    return repository;
}

void RsnapshotRawBackupJob::SetRepository(const wstring &value)
{
    repository = value;
}

wstring RsnapshotRawBackupJob::GetConfigurationFile() const
{
    return configurationFile;
}

void RsnapshotRawBackupJob::SetConfigurationFile(const wstring &value)
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
    JobStatus* backupStatus = NULL;
    if (backupJob->IsCommandAvailable())
    {
        backupStatus = backupJob->Run();
        debugManager->AddDataLine<wstring>(L"Full command output", backupJob->GetCommandOutput());
        if (backupStatus->GetCode() != JobStatus::Ok)
        {
            if (backupJob->GetCommandReturnCode() == 1)
            {
                RsnapshotErrorAnalyzer analyzer(backupJob->GetCommandOutput());
                if (analyzer.IsOutOfSpaceError())
                    backupStatus->SetDescription(L"Not enough disk space to perform backup");
                else if (analyzer.IsInvalidFolderError())
                    backupStatus->SetDescription(L"Tried to backup invalid folder");
                else
                {
                    backupStatus->SetDescription(L"Fatal error. See attached file.");
                    backupStatus->AddFileBuffer(GetAttachmentName(), backupJob->GetCommandOutput());
                }
            }
            else if (backupJob->GetCommandReturnCode() == 2)
            {
                backupStatus->SetDescription(L"RSnapshot executed with some warnings. See attached file.");
                backupStatus->SetCode(JobStatus::OkWithWarnings);
                backupStatus->AddFileBuffer(GetAttachmentName(), backupJob->GetCommandOutput());

                const wstring configurationAttachment = GetName() + L" - ConfigurationFile.txt";
                const wstring configurationContent = FileTools::GetTextFileContent(configurationFile);
                backupStatus->AddFileBuffer(configurationAttachment, configurationContent);
            }
            else
            {
                wstring description(L"Unknown error running rsnapshot. Return code : ");
                wstringstream stream;
                stream << backupJob->GetCommandReturnCode();
                description += stream.str();
                backupStatus->SetDescription(description);
                backupStatus->AddFileBuffer(GetAttachmentName(), backupJob->GetCommandOutput());
            }
        }
    }
    else
        backupStatus = new JobStatus(JobStatus::Error, L"Rsnapshot not installed");

    delete backupJob;
    return debugManager->UpdateStatus(backupStatus);
}

JobStatus *RsnapshotRawBackupJob::RunReportCreation()
{
    if (IsFirstBackup())
        return RunRecursiveListReportCreation();
    else
    {
        JobStatus* status = RunRsnapshotDiffReportCreation();
        if (status->GetCode() == JobStatus::Ok)
        {
            delete status;
            return CreateParsedReportStatus();
        }
        else
            return status;
    }
}

JobStatus *RsnapshotRawBackupJob::RunRecursiveListReportCreation()
{
    const wstring backupFolder = repository + L"/weekly.0";
    vector<wstring> fileList;
    bool ok = Tools::GetFolderContentRecursively(backupFolder, fileList);
    if (ok)
    {
        FileBackupReport report;
        report.AddAsAdded(fileList);
        JobStatus* status = new JobStatus(JobStatus::Ok, report.GetMiniDescription());
        status->AddFileBuffer(GetAttachmentName(), report.GetFullDescription());
        return debugManager->UpdateStatus(status);
    }
    else
    {
        debugManager->AddDataLine<wstring>(L"Backup folder not found", backupFolder);
        return debugManager->CreateStatus(JobStatus::OkWithWarnings, L"Report could not be created");
    }
}

JobStatus *RsnapshotRawBackupJob::RunRsnapshotDiffReportCreation()
{
    ConsoleJob* reportJob = CreateReportCommandJob();
    JobStatus* reportStatus = NULL;
    if (reportJob->IsCommandAvailable())
    {
        reportStatus = reportJob->Run();
        debugManager->AddDataLine<wstring>(L"Full report output", reportJob->GetCommandOutput());
        if (reportStatus->GetCode() == JobStatus::Ok)
            reportJobOutput = reportJob->GetCommandOutput();
        else
        {
            wstring description(L"Error creating rsnapshot report. Return code : ");
            wstringstream stream;
            stream << reportJob->GetCommandReturnCode();
            description += stream.str();
            reportStatus->SetDescription(description);
            reportStatus->SetCode(JobStatus::OkWithWarnings);
            reportStatus->AddFileBuffer(GetAttachmentName(), reportJob->GetCommandOutput());
        }
    }
    else
        reportStatus = new JobStatus(JobStatus::OkWithWarnings, L"Rsnapshot reporting not available");

    delete reportJob;
    return reportStatus;
}

JobStatus *RsnapshotRawBackupJob::CreateParsedReportStatus()
{
    RSnapshotReportParser parser;
    parser.ParseBuffer(reportJobOutput);

    JobStatus* status = new JobStatus(JobStatus::Ok, parser.GetMiniDescription());
    status->AddFileBuffer(GetAttachmentName(), parser.GetFullDescription());
    return debugManager->UpdateStatus(status);
}

bool RsnapshotRawBackupJob::IsFirstBackup() const
{
    return !FileTools::FolderExists(repository + L"/weekly.1");
}

ConsoleJob *RsnapshotRawBackupJob::CreateBackupCommandJob()
{
    const wstring parameters = wstring(L"-c ") + configurationFile + L" weekly";
    return new ConsoleJob(L"rsnapshot", parameters);
}

ConsoleJob *RsnapshotRawBackupJob::CreateReportCommandJob()
{
    const wstring parameters = wstring(L"-v ") + repository + L"/weekly.0 " + repository + L"/weekly.1 2>&1";
    return new ConsoleJob(L"rsnapshot-diff", parameters);
}

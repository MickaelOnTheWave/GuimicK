#include "rsnapshotbackupjob.h"

#include <fstream>
#include <sstream>
#include <unistd.h>

#include "rsnapshoterroranalyzer.h"
#include "tools.h"

using namespace std;

const string DEFAULT_RSNAPSHOT_CONF_FILE    = "rsnapshot.conf";
const string debugFilename                  = "RsnapshotDebug.txt";

RsnapshotBackupJob::RsnapshotBackupJob(const string& _backupRepositoryPath, const string &_rsnapshotConfFile)
    : AbstractBackupJob(), waitAfterRun(false)
{
    backupCommand = new ConsoleJob("rsnapshot");
    reportCommand = new ConsoleJob("rsnapshot-diff");

    if (_backupRepositoryPath != "")
        SetRepositoryPath(_backupRepositoryPath);

    if (_rsnapshotConfFile != "")
        SeConfigurationFile(_rsnapshotConfFile);
    else
        SeConfigurationFile(DEFAULT_RSNAPSHOT_CONF_FILE);

}

RsnapshotBackupJob::RsnapshotBackupJob(const RsnapshotBackupJob &other)
    : AbstractBackupJob(other),
      configurationFile(other.configurationFile),
      backupRepositoryPath(other.backupRepositoryPath),
      waitAfterRun(other.waitAfterRun)
{
    backupCommand = dynamic_cast<ConsoleJob*>(other.backupCommand->Clone());
    reportCommand = dynamic_cast<ConsoleJob*>(other.reportCommand->Clone());
}

RsnapshotBackupJob::~RsnapshotBackupJob()
{
	delete backupCommand;
	delete reportCommand;
}

string RsnapshotBackupJob::GetName()
{
    return "Rsnapshot Backup";
}

AbstractJob *RsnapshotBackupJob::Clone()
{
    return new RsnapshotBackupJob(*this);
}

void RsnapshotBackupJob::SetRepositoryPath(const string &path)
{
    backupRepositoryPath = path;

    string parameters("-v ");
    parameters += backupRepositoryPath + "/weekly.0 " + backupRepositoryPath + "/weekly.1 2>&1";
    reportCommand->SetCommandParameters(parameters);
}

void RsnapshotBackupJob::SeConfigurationFile(const string &file)
{
    configurationFile = file;

    string parameters("-c ");
    parameters += configurationFile + " weekly";
    backupCommand->SetCommandParameters(parameters);
}

bool RsnapshotBackupJob::InitializeFromClient(Client *client)
{
    return AbstractBackupJob::InitializeFromClient(client);
}

bool RsnapshotBackupJob::IsInitialized()
{
    bool ret = (backupRepositoryPath != "" &&
            backupCommand->IsInitialized() && reportCommand->IsInitialized());
    return ret;
}

JobStatus* RsnapshotBackupJob::Run()
{
    if (waitAfterRun)
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

void RsnapshotBackupJob::SetWaitAfterRun(const bool value)
{
    waitAfterRun = value;
}

void RsnapshotBackupJob::RunRepositoryBackup(const string &source, const string &destination, AbstractBackupJob::ResultCollection &results)
{
    // TODO : Finish refactoring. This should not be empty.
}

JobStatus *RsnapshotBackupJob::CreateGlobalStatus(const AbstractBackupJob::ResultCollection &results)
{
    return NULL;
}

JobStatus *RsnapshotBackupJob::RunBackup()
{
    JobStatus* backupStatus = backupCommand->Run();
    debugManager->AddDataLine<string>("Full command output", backupCommand->GetCommandOutput());
    if (backupStatus->GetCode() != JobStatus::OK)
    {
        if (backupCommand->GetCommandReturnCode() == 1)
        {
            RsnapshotErrorAnalyzer analyzer(backupCommand->GetCommandOutput());
            if (analyzer.IsOutOfSpaceError())
                backupStatus->SetDescription("Not enough disk space to perform backup");
            else if (analyzer.IsInvalidFolderError())
                backupStatus->SetDescription("Tried to backup invalid folder");
            else
            {
                backupStatus->SetDescription("Fatal error. See attached file.");
                backupStatus->AddFileBuffer(GetAttachmentName(), backupCommand->GetCommandOutput());
            }
        }
        else if (backupCommand->GetCommandReturnCode() == 2)
        {
            backupStatus->SetDescription("RSnapshot executed with some warnings. See attached file.");
            backupStatus->SetCode(JobStatus::OK_WITH_WARNINGS);
            backupStatus->AddFileBuffer(GetAttachmentName(), backupCommand->GetCommandOutput());
        }
        else
        {
            string description("Unknown error running rsnapshot. Return code : ");
            stringstream stream;
            stream << backupCommand->GetCommandReturnCode();
            description += stream.str();
            backupStatus->SetDescription(description);
            backupStatus->AddFileBuffer(GetAttachmentName(), backupCommand->GetCommandOutput());
        }
    }

    return debugManager->UpdateStatus(backupStatus);
}

JobStatus *RsnapshotBackupJob::RunReportCreation()
{
    JobStatus* reportStatus = reportCommand->Run();
    debugManager->AddDataLine<string>("Full report output", reportCommand->GetCommandOutput());
    if (reportStatus->GetCode() != JobStatus::OK)
    {
        string description("Error creating rsnapshot report. Return code : ");
        stringstream stream;
        stream << reportCommand->GetCommandReturnCode();
        description += stream.str();
        reportStatus->SetDescription(description);
        reportStatus->SetCode(JobStatus::OK_WITH_WARNINGS);
        reportStatus->AddFileBuffer(GetAttachmentName(), backupCommand->GetCommandOutput());
    }

    return debugManager->UpdateStatus(reportStatus);
}

JobStatus *RsnapshotBackupJob::CreateParsedReportStatus()
{
    RSnapshotReportParser parser;
    parser.ParseBuffer(reportCommand->GetCommandOutput());

    JobStatus* status = new JobStatus(JobStatus::OK, parser.GetMiniDescription());
    status->AddFileBuffer(GetAttachmentName(), parser.GetFullDescription());
    return debugManager->UpdateStatus(status);
}

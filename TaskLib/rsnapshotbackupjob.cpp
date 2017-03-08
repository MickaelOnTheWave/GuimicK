#include "rsnapshotbackupjob.h"

#include <fstream>
#include <sstream>
#include <unistd.h>

#include "rsnapshoterroranalyzer.h"
#include "tools.h"

using namespace std;

const string DEFAULT_BACKUP_FILENAME        = "FilesystemBackup.txt";
const string DEFAULT_RSNAPSHOT_CONF_FILE    = "rsnapshot.conf";
const string debugFilename                  = "RsnapshotDebug.txt";

RsnapshotBackupJob::RsnapshotBackupJob(const string& _backupRepositoryPath, const string &_rsnapshotConfFile)
    : showDebugOutput(false), waitAfterRun(false)
{
    backupCommand = new ConsoleJob("", "");
    reportCommand = new ConsoleJob("", "");

    if (_backupRepositoryPath != "")
        SetRepositoryPath(_backupRepositoryPath);

    if (_rsnapshotConfFile != "")
        SeConfigurationFile(_rsnapshotConfFile);
    else
        SeConfigurationFile(DEFAULT_RSNAPSHOT_CONF_FILE);

}

RsnapshotBackupJob::~RsnapshotBackupJob()
{
	delete backupCommand;
	delete reportCommand;
}

string RsnapshotBackupJob::GetName()
{
    return "Backup";
}

AbstractJob *RsnapshotBackupJob::Clone()
{
    RsnapshotBackupJob* clone = new RsnapshotBackupJob();
    clone->backupCommand = static_cast<ConsoleJob*>(backupCommand->Clone());
    clone->reportCommand = static_cast<ConsoleJob*>(reportCommand->Clone());
    clone->backupRepositoryPath = backupRepositoryPath;
    clone->configurationFile = configurationFile;
    clone->showDebugOutput = showDebugOutput;
    clone->waitAfterRun = waitAfterRun;
    return clone;
}

void RsnapshotBackupJob::SetRepositoryPath(const string &path)
{
    backupRepositoryPath = path;

    string command("rsnapshot-diff -v ");
    command += backupRepositoryPath + "/weekly.0 " + backupRepositoryPath + "/weekly.1 2>&1";
    reportCommand->Initialize(command);
}

void RsnapshotBackupJob::SeConfigurationFile(const string &file)
{
    configurationFile = file;

    string command("rsnapshot -c ");
    command += configurationFile + " weekly";
    backupCommand->Initialize(command);
    backupCommand->SetOutputToBuffer();
}

bool RsnapshotBackupJob::InitializeFromClient(Client *)
{
    return true;
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

    stringstream debugOutput;
	JobStatus* backupStatus = backupCommand->Run();
    if (showDebugOutput)
        debugOutput << "Full command output : <" << backupCommand->GetCommandOutput() << ">" << endl;
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
                backupStatus->SetDescription("Fatal error running RSnapshot. See attached file.");
                backupStatus->AddFileBuffer(DEFAULT_BACKUP_FILENAME, backupCommand->GetCommandOutput());
            }
        }
        else if (backupCommand->GetCommandReturnCode() == 2)
        {
            backupStatus->SetDescription("RSnapshot executed with some warnings. See attached file.");
            backupStatus->SetCode(JobStatus::OK_WITH_WARNINGS);
            backupStatus->AddFileBuffer(DEFAULT_BACKUP_FILENAME, backupCommand->GetCommandOutput());
        }
		else
		{
            string description("Unknown error running rsnapshot. Return code : ");
			stringstream stream;
			stream << backupCommand->GetCommandReturnCode();
			description += stream.str();
			backupStatus->SetDescription(description);
            backupStatus->AddFileBuffer(DEFAULT_BACKUP_FILENAME, backupCommand->GetCommandOutput());
		}
		return backupStatus;
	}
	else
		delete backupStatus;

	JobStatus* reportStatus = reportCommand->Run();
    if (showDebugOutput)
        debugOutput << "Full report output : <" << reportCommand->GetCommandOutput() << ">" << endl;
	if (reportStatus->GetCode() != JobStatus::OK)
	{
		string description("Error creating rsnapshot report. Return code : ");
		stringstream stream;
		stream << reportCommand->GetCommandReturnCode();
		description += stream.str();
		reportStatus->SetDescription(description);
		reportStatus->SetCode(JobStatus::OK_WITH_WARNINGS);
        reportStatus->AddFileBuffer(DEFAULT_BACKUP_FILENAME, backupCommand->GetCommandOutput());
		return reportStatus;
	}
	else
		delete reportStatus;

    RSnapshotReportParser parser;
    parser.ParseBuffer(reportCommand->GetCommandOutput());

    JobStatus* status = new JobStatus(JobStatus::OK, parser.GetMiniDescription());
    status->AddFileBuffer(DEFAULT_BACKUP_FILENAME, parser.GetFullDescription());
    if (showDebugOutput)
        status->AddFileBuffer(debugFilename, debugOutput.str());

    return status;
}

void RsnapshotBackupJob::SetOutputDebugInformation(const bool value)
{
    showDebugOutput = value;
}

void RsnapshotBackupJob::SetWaitAfterRun(const bool value)
{
    waitAfterRun = value;
}

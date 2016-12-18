#include "backupjob.h"

#include <fstream>
#include <sstream>
#include <tools.h>

#include <iostream>

using namespace std;

const string DEFAULT_BACKUP_FILENAME        = "FilesystemBackup.txt";
const string DEFAULT_RSNAPSHOT_CONF_FILE    = "rsnapshot.conf";

BackupJob::BackupJob(const string& _backupRepositoryPath, const string &_rsnapshotConfFile)
{
    backupCommand = new ConsoleJob("", "", "");
    reportCommand = new ConsoleJob("", "", "");

    if (_backupRepositoryPath != "")
        SetRepositoryPath(_backupRepositoryPath);

    if (_rsnapshotConfFile != "")
        SetRsnapshotConfFile(_rsnapshotConfFile);
    else
        SetRsnapshotConfFile(DEFAULT_RSNAPSHOT_CONF_FILE);

}

BackupJob::~BackupJob()
{
	delete backupCommand;
	delete reportCommand;
}

string BackupJob::GetName()
{
    return "Backup";
}

AbstractJob *BackupJob::Clone()
{
    BackupJob* clone = new BackupJob();
    clone->backupCommand = static_cast<ConsoleJob*>(backupCommand->Clone());
    clone->reportCommand = static_cast<ConsoleJob*>(reportCommand->Clone());
    clone->backupRepositoryPath = backupRepositoryPath;
    clone->rsnapshotConfFile = rsnapshotConfFile;
    return clone;
}

void BackupJob::SetRepositoryPath(const string &path)
{
    backupRepositoryPath = path;

    string reportCommandParams("-v ");
    reportCommandParams += backupRepositoryPath + "/weekly.0 " + backupRepositoryPath + "/weekly.1 2>&1";
    reportCommand->Initialize("rsnapshot-diff", reportCommandParams);
}

void BackupJob::SetRsnapshotConfFile(const string &file)
{
    rsnapshotConfFile = file;

    string commandParams("-c ");
    commandParams += rsnapshotConfFile + " weekly";
    backupCommand->Initialize("rsnapshot", commandParams);
    backupCommand->SetOutputToBuffer();
}

bool BackupJob::InitializeFromClient(Client *)
{
    return true;
}

bool BackupJob::IsInitialized()
{
    bool ret = (backupRepositoryPath != "" &&
            backupCommand->IsInitialized() && reportCommand->IsInitialized());

    if (!ret)
    {
        cout << "Backup job initialization is failing : " << endl;
        cout << "\tRepository path : " << backupRepositoryPath << endl;
        cout << "\tRsnapshot conf file : " << rsnapshotConfFile << endl;
        cout << "\tBackup command ok?" << backupCommand->IsInitialized() << endl;
        cout << "\tReport command ok?" << reportCommand->IsInitialized() << endl;
    }

    return ret;
}

JobStatus* BackupJob::Run()
{
	JobStatus* backupStatus = backupCommand->Run();
	if (backupStatus->GetCode() != JobStatus::OK)
	{
        if (backupCommand->GetCommandReturnCode() == 1)
            backupStatus->SetDescription("Fatal error running RSnapshot. See attached file.");
        else if (backupCommand->GetCommandReturnCode() == 2)
        {
            backupStatus->SetDescription("RSnapshot executed with some warnings. See attached file.");
            backupStatus->SetCode(JobStatus::OK_WITH_WARNINGS);
        }
		else
		{
            string description("Unknown error running rsnapshot. Return code : ");
			stringstream stream;
			stream << backupCommand->GetCommandReturnCode();
			description += stream.str();
			backupStatus->SetDescription(description);
		}
        backupStatus->AddFileBuffer(DEFAULT_BACKUP_FILENAME, backupCommand->GetCommandOutput());
		return backupStatus;
	}
	else
		delete backupStatus;

	JobStatus* reportStatus = reportCommand->Run();
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
    parser.ParseBuffer(backupCommand->GetCommandOutput());

    JobStatus* status = new JobStatus(JobStatus::OK, parser.GetMiniDescription());
    status->AddFileBuffer(DEFAULT_BACKUP_FILENAME, parser.GetFullDescription());
	return status;
}


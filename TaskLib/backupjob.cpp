#include "backupjob.h"

#include <fstream>
#include <sstream>
#include <tools.h>

#include <iostream>

using namespace std;

const string BACKUP_REPORT_FILE             = "backupreport.txt";
const string BACKUP_ERROR_FILE              = "backuperrors.txt";
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

void BackupJob::SetRepositoryPath(const string &path)
{
    backupRepositoryPath = path;

    string reportCommandParams("-v ");
    reportCommandParams += backupRepositoryPath + "/weekly.0 " + backupRepositoryPath + "/weekly.1 > ";
    reportCommandParams += BACKUP_REPORT_FILE + " 2> " + BACKUP_ERROR_FILE;
    reportCommand->Initialize("rsnapshot-diff", reportCommandParams);
}

void BackupJob::SetRsnapshotConfFile(const string &file)
{
    rsnapshotConfFile = file;

    string commandParams("-c ");
    commandParams += rsnapshotConfFile + " weekly";
    backupCommand->Initialize("rsnapshot", commandParams);
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
        backupStatus->AddFile(BACKUP_ERROR_FILE);
		backupStatus->AddFile(BACKUP_REPORT_FILE);
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
		reportStatus->AddFile(BACKUP_REPORT_FILE);
		return reportStatus;
	}
	else
		delete reportStatus;

    RSnapshotReportParser parser;
    string shortReport;
    parser.ParseUsingFiles(BACKUP_REPORT_FILE, BACKUP_REPORT_FILE, shortReport);

	JobStatus* status = new JobStatus(JobStatus::OK, shortReport);
	status->AddFile(BACKUP_REPORT_FILE);
	return status;
}


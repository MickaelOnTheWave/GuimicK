#include "clamavjob.h"

#include <sstream>
#include "clamavreportparser.h"

using namespace std;

const string UPDATE_LOG_FILE = "clamavupdate.txt";
const string SCAN_LOG_FILE = "clamavscan.txt";

ClamAvJob::ClamAvJob()
   : AbstractJob("ClamAV Scan"),
     scanDir(""), blockOnFailingUpdate(false)
{
    virusDefinitionUpdateJob = new UserConsoleJob("", "freshclam.exe --verbose --on-update-execute=EXIT_0 --on-error-execute=EXIT_1 --on-outdated-execute=EXIT_2");
    virusFullScanJob = new UserConsoleJob("", "");
}

ClamAvJob::~ClamAvJob()
{
	delete virusDefinitionUpdateJob;
	delete virusFullScanJob;
}

AbstractJob *ClamAvJob::Clone()
{
    ClamAvJob* clone = new ClamAvJob();
    clone->virusDefinitionUpdateJob = static_cast<UserConsoleJob*>(virusDefinitionUpdateJob->Clone());
    clone->virusFullScanJob = static_cast<UserConsoleJob*>(virusFullScanJob->Clone());
    clone->scanDir = scanDir;
    clone->blockOnFailingUpdate = blockOnFailingUpdate;
    return clone;
}

bool ClamAvJob::InitializeFromClient(Client *client)
{
   virusDefinitionUpdateJob->InitializeFromClient(client);
   if (!virusFullScanJob->InitializeFromClient(client))
     return false;

   if (!client->HasProperty("avscandir"))
      return false;

   scanDir = client->GetProperty("avscandir");
   string command("clamscan.exe --verbose --recursive ");
   command += scanDir;
   virusFullScanJob->Initialize(command);
   blockOnFailingUpdate = (client->GetProperty("avmandatoryupdate") == "true");
   return true;
}

bool ClamAvJob::IsInitialized()
{
    return true;
}

JobStatus *ClamAvJob::Run()
{
	JobStatus* updateStatus = virusDefinitionUpdateJob->Run();
    updateStatus->AddFileBuffer(UPDATE_LOG_FILE, virusDefinitionUpdateJob->GetCommandOutput());
	if (blockOnFailingUpdate && updateStatus->GetCode() != JobStatus::Ok)
	{
		bool blockingError = true;
		string description;
		int returnCode = virusDefinitionUpdateJob->GetCommandReturnCode();
		if (returnCode == 1)
			description = "Error while trying to update virus database.";
		else if (returnCode == 2)
		{
			description = "Virus database is outdated!";
			blockingError = false;
		}
		else if (returnCode == 40)
			description = "Unknown option passed.";
		else if (returnCode == 50)
			description = "Can't change directory.";
		else if (returnCode == 56)
			description = "Config file error.";
		else if (returnCode == 57)
			description = "Can't create new file.";
		else
		{
			description = "Unknown error. Return code : ";
			stringstream stream;
			stream << returnCode;
			description += stream.str();
		}
		updateStatus->SetDescription(description);

		if (blockingError)
			return updateStatus;
	}

	JobStatus* scanStatus = virusFullScanJob->Run();
    scanStatus->AddFileBuffer(SCAN_LOG_FILE, virusFullScanJob->GetCommandOutput());
	if (scanStatus->GetCode() != JobStatus::Ok)
	{
		string description("Error on virus scan. Return code : ");
		stringstream stream;
		stream << virusFullScanJob->GetCommandReturnCode();
		description += stream.str();
		scanStatus->SetDescription(description);
		return scanStatus;
	}

	JobStatus* finalStatus = new JobStatus(JobStatus::Ok);
    finalStatus->AddAllFilesFromStatus(updateStatus);

    ClamAvReportParser parser;
    bool parsingOk = parser.ParseBuffer(virusFullScanJob->GetCommandOutput());
    if (parsingOk)
        finalStatus->AddFileBuffer(SCAN_LOG_FILE, parser.GetFullDescription());
    else
        finalStatus->AddFileBuffer(SCAN_LOG_FILE, virusFullScanJob->GetCommandOutput());
    finalStatus->SetDescription(parser.GetMiniDescription());

    delete updateStatus;
    delete scanStatus;
	return finalStatus;
}

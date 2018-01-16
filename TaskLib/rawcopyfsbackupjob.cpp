#include "rawcopyfsbackupjob.h"

#include "tools.h"

using namespace std;

static const string errorReportCreation = "Error creating report";
static const string defaultName = "Copy Backup";

RawCopyFsBackupJob::RawCopyFsBackupJob() : AbstractCopyFsBackupJob(defaultName)
{
}

RawCopyFsBackupJob::RawCopyFsBackupJob(const AbstractCopyFsBackupJob &other)
    : AbstractCopyFsBackupJob(other)
{
}

AbstractJob *RawCopyFsBackupJob::Clone()
{
   return new RawCopyFsBackupJob(*this);
}

string RawCopyFsBackupJob::GetTypeName() const
{
   return defaultName;
}

JobStatus* RawCopyFsBackupJob::Run(const string& source, const string& destination)
{
   ConsoleJob copyCommand;
   PrepareCopyCommand(source, destination, copyCommand);
   return copyCommand.Run();
}

void RawCopyFsBackupJob::PrepareCopyCommand(const std::string &source,
                                              const std::string &destination,
                                              ConsoleJob &commandJob)
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

void RawCopyFsBackupJob::CreateReport(const std::string &destination,
                                        const std::string &/*output*/,
                                        AbstractBackupJob::ResultCollection &results)
{
    FileBackupReport* report = new FileBackupReport();
    JobStatus* status = new JobStatus();

    // TODO : change this to a proper recursive command.
    ConsoleJob lsCommand("ls", destination);
    lsCommand.RunWithoutStatus();
    if (lsCommand.GetCommandReturnCode() == 0)
    {
        vector<string> fileList;
        Tools::TokenizeString(lsCommand.GetCommandOutput(), '\n', fileList);

        report->AddAsAdded(fileList);
        status->SetCode(JobStatus::OK);
        status->SetDescription(report->GetMiniDescription());
        status->AddFileBuffer(GetAttachmentName(), report->GetFullDescription());
    }
    else
    {
        status->SetCode(JobStatus::OK_WITH_WARNINGS);
        status->SetDescription(errorReportCreation);
        status->AddFileBuffer(GetAttachmentName(), lsCommand.GetCommandOutput());
    }

    results.push_back(make_pair(status, report));
}

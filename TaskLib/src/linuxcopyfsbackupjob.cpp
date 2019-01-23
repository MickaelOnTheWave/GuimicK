#include "linuxcopyfsbackupjob.h"

#include "tools.h"

using namespace std;

static const string errorReportCreation = "Error creating report";

LinuxCopyFsBackupJob::LinuxCopyFsBackupJob() : AbstractRawCopyFsBackupJob()
{
}

LinuxCopyFsBackupJob::LinuxCopyFsBackupJob(const AbstractCopyFsBackupJob &other)
    : AbstractRawCopyFsBackupJob(other)
{
}

AbstractJob *LinuxCopyFsBackupJob::Clone()
{
   return new LinuxCopyFsBackupJob(*this);
}

void LinuxCopyFsBackupJob::PrepareCopyCommand(const std::string &source,
                                            const std::string &destination,
                                            ConsoleJob &commandJob)
{
    const string command = (target.isLocal ? "cp" : "scp");
    string params;
    if (target.isLocal)
        params = "-R ";
    else
        params = string("-r ") + target.sshUser + "@" + target.sshHost + ":";
    params += source + "/* " + destination;

    commandJob.SetCommand(command);
    commandJob.SetCommandParameters(params);
}

void LinuxCopyFsBackupJob::CreateReport(const std::string &destination,
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
        status->SetCode(JobStatus::Ok);
        status->SetDescription(report->GetMiniDescription());
        status->AddFileBuffer(GetAttachmentName(), report->GetFullDescription());
    }
    else
    {
        status->SetCode(JobStatus::OkWithWarnings);
        status->SetDescription(errorReportCreation);
        status->AddFileBuffer(GetAttachmentName(), lsCommand.GetCommandOutput());
    }

    results.push_back(make_pair(status, report));
}

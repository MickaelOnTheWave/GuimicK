#include "linuxcopyfsbackupjob.h"

#include "stringtools.h"

using namespace std;

static const wstring errorReportCreation = L"Error creating report";

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

void LinuxCopyFsBackupJob::PrepareCopyCommand(  const std::wstring &source,
                                                const std::wstring &destination,
                                                ConsoleJob &commandJob)
{
    const wstring command = (target.isLocal ? L"cp" : L"scp");
    wstring params;
    if (target.isLocal)
        params = L"-R ";
    else
        params = wstring(L"-r ") + target.sshUser + L"@" + target.sshHost + L":";
    params += source + L"/* " + destination;

    commandJob.SetCommand(command);
    commandJob.SetCommandParameters(params);
}

void LinuxCopyFsBackupJob::CreateReport(const std::wstring &destination,
                                        const std::wstring &/*output*/,
                                        AbstractBackupJob::ResultCollection &results)
{
    FileBackupReport* report = new FileBackupReport();
    BackupJobStatus status;

    // TODO : change this to a proper recursive command.
    ConsoleJob lsCommand(L"ls", destination);
    lsCommand.RunWithoutStatus();
    if (lsCommand.GetCommandReturnCode() == 0)
    {
        vector<wstring> fileList;
        StringTools::Tokenize(lsCommand.GetCommandOutput(), '\n', fileList);

        report->AddAsAdded(fileList);
        status.SetCode(JobStatus::Ok);
        status.SetDescription(report->GetMiniDescription());
        status.AddFileBuffer(GetAttachmentName(), report->GetFullDescription());
        status.SetFileReport(report);
    }
    else
    {
        status.SetCode(JobStatus::OkWithWarnings);
        status.SetDescription(errorReportCreation);
        status.AddFileBuffer(GetAttachmentName(), lsCommand.GetCommandOutput());
    }

    results.push_back(status);
}

#include "WindowsCopyFsBackupJob.h"

#include "tools.h"

using namespace std;

WindowsCopyFsBackupJob::WindowsCopyFsBackupJob() : AbstractRawCopyFsBackupJob()
{
}

WindowsCopyFsBackupJob::WindowsCopyFsBackupJob(const AbstractCopyFsBackupJob &other)
    : AbstractRawCopyFsBackupJob(other)
{
}

AbstractJob *WindowsCopyFsBackupJob::Clone()
{
   return new WindowsCopyFsBackupJob(*this);
}

void WindowsCopyFsBackupJob::PrepareCopyCommand(const std::wstring &source,
                                            const std::wstring &destination,
                                            ConsoleJob &commandJob)
{
    const wstring params = source + L" " + destination + L" /e /y";
    commandJob.SetCommand(L"xcopy");
    commandJob.SetCommandParameters(params);
}

void WindowsCopyFsBackupJob::CreateReport(const std::wstring&,
                                        const std::wstring &output,
                                        AbstractBackupJob::ResultCollection &results)
{
   FileBackupReport* report = CreateBackupReport(output);
   JobStatus* status = new JobStatus(JobStatus::Ok, L"");
   status->SetDescription(report->GetMiniDescription());
   status->AddFileBuffer(GetAttachmentName(), report->GetFullDescription());
   results.push_back(make_pair(status, report));
}

FileBackupReport* WindowsCopyFsBackupJob::CreateBackupReport(const wstring& output) const
{
   vector<wstring> fileList;
   Tools::TokenizeString(output, '\n', fileList);
   fileList.pop_back();

   FileBackupReport* report = new FileBackupReport();
   report->AddAsAdded(fileList);
   return report;
}

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

void WindowsCopyFsBackupJob::PrepareCopyCommand(const std::string &source,
                                            const std::string &destination,
                                            ConsoleJob &commandJob)
{
    const string params = source + " " + destination + " /e /y";
    commandJob.SetCommand("xcopy");
    commandJob.SetCommandParameters(params);
}

void WindowsCopyFsBackupJob::CreateReport(const std::string&,
                                        const std::string &output,
                                        AbstractBackupJob::ResultCollection &results)
{
   FileBackupReport* report = CreateBackupReport(output);
   JobStatus* status = new JobStatus(JobStatus::Ok, "");
   status->SetDescription(report->GetMiniDescription());
   status->AddFileBuffer(GetAttachmentName(), report->GetFullDescription());
   results.push_back(make_pair(status, report));
}

FileBackupReport* WindowsCopyFsBackupJob::CreateBackupReport(const string& output) const
{
   vector<string> fileList;
   Tools::TokenizeString(output, '\n', fileList);
   fileList.pop_back();

   FileBackupReport* report = new FileBackupReport();
   report->AddAsAdded(fileList);
   return report;
}

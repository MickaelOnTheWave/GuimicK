#include "rsynccopyfsbackupjob.h"

#include "pathtools.h"
#include "rsynccommandparser.h"

using namespace std;

static const wstring defaultName = L"Rsync Copy Backup";

RsyncCopyFsBackupJob::RsyncCopyFsBackupJob() : AbstractCopyFsBackupJob(defaultName)
{
}

RsyncCopyFsBackupJob::RsyncCopyFsBackupJob(const AbstractCopyFsBackupJob &other)
    : AbstractCopyFsBackupJob(other)
{
}

AbstractJob *RsyncCopyFsBackupJob::Clone()
{
   return new RsyncCopyFsBackupJob(*this);
}

wstring RsyncCopyFsBackupJob::GetTypeName() const
{
   return defaultName;
}

bool RsyncCopyFsBackupJob::IsAvailable()
{
    const wstring path = PathTools::GetCommandPath(L"rsync", ConsoleJob::appSearchPaths);
    return (path != L"");
}

bool RsyncCopyFsBackupJob::IsInitialized()
{
   return AbstractCopyFsBackupJob::IsInitialized() && IsAvailable();
}

void RsyncCopyFsBackupJob::GetExclusionList(std::vector<wstring>& exclusions)
{
   exclusions = itemsToExclude;
}

void RsyncCopyFsBackupJob::AddToExclusions(const wstring &item)
{
   itemsToExclude.push_back(item);
}

void RsyncCopyFsBackupJob::ClearExclusionList()
{
   itemsToExclude.clear();
}

void RsyncCopyFsBackupJob::PrepareCopyCommand(const std::wstring &source,
                                              const std::wstring &destination,
                                              ConsoleJob &commandJob)
{
    commandJob.SetCommand(L"rsync");
    wstring params = L"-avzhi --delete " + BuildExclusions() + L" ";
    params += BuildSource(source) + L" " + destination;
    debugManager->AddDataLine<wstring>(L"Rsync params", params);
    commandJob.SetCommandParameters(params);
}

void RsyncCopyFsBackupJob::CreateReport(const std::wstring &,
                                        const std::wstring &output,
                                        AbstractBackupJob::ResultCollection &results)
{
    JobStatus* status = new JobStatus(JobStatus::Ok);
    FileBackupReport* report = new FileBackupReport();

    RsyncCommandParser parser;
    bool result = parser.ParseBuffer(output);
    if (result)
    {
        status->SetDescription(parser.GetMiniDescription());
        status->AddFileBuffer(L"RsyncCopy.txt", parser.GetFullDescription());
        parser.GetReport(*report);
        results.push_back(BackupJobStatus(*status, report));
    }
    else
    {
        status->SetCode(JobStatus::OkWithWarnings);
        status->SetDescription(L"Report parsing failed");
        results.push_back(BackupJobStatus(*status, NULL));
    }
}

wstring RsyncCopyFsBackupJob::BuildSource(const wstring &originalSource) const
{
    wstring finalSource;
    if (target.isLocal)
        finalSource = originalSource;
    else
        finalSource = target.sshUser + L"@" + target.sshHost + L":" + originalSource;
    return finalSource + L"/";
}

wstring RsyncCopyFsBackupJob::BuildExclusions() const
{
    if (itemsToExclude.size() == 0)
        return wstring(L"");

    wstring parameter(L"");
    vector<wstring>::const_iterator it=itemsToExclude.begin();
    for (; it!=itemsToExclude.end(); ++it)
        parameter += wstring(L"--exclude '") + *it + L"'";

    return parameter;
}

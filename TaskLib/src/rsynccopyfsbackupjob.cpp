#include "rsynccopyfsbackupjob.h"

#include "pathtools.h"
#include "rsynccommandparser.h"

using namespace std;

static const string defaultName = "Rsync Copy Backup";

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

string RsyncCopyFsBackupJob::GetTypeName() const
{
   return defaultName;
}

bool RsyncCopyFsBackupJob::IsAvailable()
{
    const string path = PathTools::GetCommandPath("rsync", ConsoleJob::appSearchPaths);
    return (path != "");
}

bool RsyncCopyFsBackupJob::IsInitialized()
{
   return AbstractCopyFsBackupJob::IsInitialized() && IsAvailable();
}

void RsyncCopyFsBackupJob::GetExclusionList(std::vector<string>& exclusions)
{
   exclusions = itemsToExclude;
}

void RsyncCopyFsBackupJob::AddToExclusions(const string &item)
{
   itemsToExclude.push_back(item);
}

void RsyncCopyFsBackupJob::ClearExclusionList()
{
   itemsToExclude.clear();
}

void RsyncCopyFsBackupJob::PrepareCopyCommand(const std::string &source,
                                              const std::string &destination,
                                              ConsoleJob &commandJob)
{
    commandJob.SetCommand("rsync");
    string params = "-avzhi --delete " + BuildExclusions() + " ";
    params += BuildSource(source) + " " + destination;
    debugManager->AddDataLine<string>("Rsync params", params);
    commandJob.SetCommandParameters(params);
}

void RsyncCopyFsBackupJob::CreateReport(const std::string &,
                                        const std::string &output,
                                        AbstractBackupJob::ResultCollection &results)
{
    JobStatus* status = new JobStatus(JobStatus::Ok);
    FileBackupReport* report = new FileBackupReport();

    RsyncCommandParser parser;
    bool result = parser.ParseBuffer(output);
    if (result)
    {
        status->SetDescription(parser.GetMiniDescription());
        status->AddFileBuffer("RsyncCopy.txt", parser.GetFullDescription());
        parser.GetReport(*report);
        results.push_back(make_pair(status, report));
    }
    else
    {
        status->SetCode(JobStatus::OkWithWarnings);
        status->SetDescription("Report parsing failed");
        results.push_back(make_pair(status, new FileBackupReport()));
    }
}

string RsyncCopyFsBackupJob::BuildSource(const string &originalSource) const
{
    string finalSource;
    if (target.isLocal)
        finalSource = originalSource;
    else
        finalSource = target.sshUser + "@" + target.sshHost + ":" + originalSource;
    return finalSource + "/";
}

string RsyncCopyFsBackupJob::BuildExclusions() const
{
    if (itemsToExclude.size() == 0)
        return string("");

    string parameter("");
    vector<string>::const_iterator it=itemsToExclude.begin();
    for (; it!=itemsToExclude.end(); ++it)
        parameter += string("--exclude '") + *it + "'";

    return parameter;
}

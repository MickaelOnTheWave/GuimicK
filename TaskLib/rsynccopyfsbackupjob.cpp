#include "rsynccopyfsbackupjob.h"

#include "rsynccommandparser.h"
#include "tools.h"

using namespace std;

const string defaultDebugFilename = "RsyncCopyFsBackup.txt";

RsyncCopyFsBackupJob::RsyncCopyFsBackupJob() : AbstractCopyFsBackupJob(defaultDebugFilename)
{
}

RsyncCopyFsBackupJob::RsyncCopyFsBackupJob(const std::string &debugFilename)
    : AbstractCopyFsBackupJob(debugFilename)
{
}

RsyncCopyFsBackupJob::RsyncCopyFsBackupJob(const AbstractCopyFsBackupJob &other)
    : AbstractCopyFsBackupJob(other)
{
}

string RsyncCopyFsBackupJob::GetName()
{
    return string("RSync Fs Backup");
}

AbstractJob *RsyncCopyFsBackupJob::Clone()
{
    return new RsyncCopyFsBackupJob(*this);
}

bool RsyncCopyFsBackupJob::IsAvailable()
{
    const string path = Tools::GetCommandPath("rsync", ConsoleJob::appSearchPaths);
    return (path != "");
}

bool RsyncCopyFsBackupJob::IsInitialized()
{
    return AbstractCopyFsBackupJob::IsInitialized() && IsAvailable();
}

void RsyncCopyFsBackupJob::AddToExclusions(const string &item)
{
    itemsToExclude.push_back(item);
}

void RsyncCopyFsBackupJob::PrepareCopyCommand(const std::string &source,
                                              const std::string &destination,
                                              ConsoleJob &commandJob)
{
    commandJob.SetCommand("rsync");
    string params = "-avzhi --delete " + BuildExclusions() + " ";
    params += BuildSource(source) + " " + destination.substr(0, destination.size()-1);
    debugManager.AddDataLine<string>("Rsync params", params);
    commandJob.SetCommandParameters(params);
}

void RsyncCopyFsBackupJob::CreateReport(const std::string &,
                                        const std::string &output,
                                        AbstractBackupJob::ResultCollection &results)
{
    JobStatus* status = new JobStatus(JobStatus::OK);
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
        status->SetCode(JobStatus::OK_WITH_WARNINGS);
        status->SetDescription("Report parsing failed");
        results.push_back(make_pair(status, new FileBackupReport()));
    }
}

string RsyncCopyFsBackupJob::BuildSource(const string &originalSource) const
{
    string finalSource;
    if (isTargetLocal)
        finalSource = originalSource;
    else
        finalSource = sshUser + "@" + sshHost + ":" + originalSource;
    return finalSource;
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

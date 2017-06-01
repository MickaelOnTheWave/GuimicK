#include "rsynccopyfsbackupjob.h"

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

void RsyncCopyFsBackupJob::PrepareCopyCommand(const std::string &source,
                                              const std::string &destination,
                                              ConsoleJob &commandJob)
{
    commandJob.SetCommand("rsync");
    string params;
    if (isTargetLocal)
        params = "-avzhi --delete ";

    params += source + " " + destination.substr(0, destination.size()-1);

    commandJob.SetCommandParameters(params);
}

void RsyncCopyFsBackupJob::CreateReport(const std::string &destination,
                                        const std::string &output,
                                        AbstractBackupJob::ResultCollection &results)
{
    // see http://web.archive.org/web/20160904174444/http://andreafrancia.it/2010/03/understanding-the-output-of-rsync-itemize-changes.html

    // Basically :
    // run rsync with -i
    // Check for lines starting with *, c or >
    // if tag is *deleting -> to delete list
    // if tag is >f+++++ or cd++++ -> to add list
    // else to modified list
}

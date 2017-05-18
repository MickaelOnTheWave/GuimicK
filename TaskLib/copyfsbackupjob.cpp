#include "copyfsbackupjob.h"

#include "consolejob.h"

using namespace std;

CopyFsBackupJob::CopyFsBackupJob() : AbstractBackupJob(),
    debugManager("CopyFsBackup", false)
{
}

CopyFsBackupJob::CopyFsBackupJob(const CopyFsBackupJob &other)
    : AbstractBackupJob(other),
      debugManager(other.debugManager)
{
}

std::string CopyFsBackupJob::GetName()
{
    return string("CopyFsBackup");
}

AbstractJob *CopyFsBackupJob::Clone()
{
    return new CopyFsBackupJob(*this);
}

JobStatus *CopyFsBackupJob::Run()
{
    vector<pair<JobStatus*, FileBackupReport*> > results;
    vector<pair<string, string> >::const_iterator it=folderList.begin();
    for (; it!=folderList.end(); it++)
        results.push_back(RunRepositoryBackup(it->first, it->second));

    //return debugManager.UpdateStatus(CreateGlobalStatus(results));
    return new JobStatus(JobStatus::ERROR, "Implementation not ready");
}

int CopyFsBackupJob::RunOnParameters(const string &source, const string &destination)
{
    const string command = (isTargetLocal ? "cp" : "scp");
    string params;
    if (isTargetLocal)
        params = "-R ";
    else
        params = string("-r ") + sshUser + "@" + sshHost + ":";
    params += source + "* " + destination;

    ConsoleJob commandJob(command, params);
    commandJob.RunWithoutStatus();

    debugManager.AddStringDataLine("Copy command", command + " " + params);
    debugManager.AddStringDataLine("Copy output", commandJob.GetCommandOutput());
    debugManager.AddIntDataLine("Copy value", commandJob.GetCommandReturnCode());

    return commandJob.GetCommandReturnCode();
}

pair<JobStatus*, FileBackupReport*> CopyFsBackupJob::RunRepositoryBackup(
        const string &source,
        const string &destination)
{
    return pair<JobStatus*, FileBackupReport*>(NULL, NULL);
}

#include "copyfsbackupjob.h"

#include "consolejob.h"

using namespace std;

CopyFsBackupJob::CopyFsBackupJob() : AbstractBackupJob("CopyFsBackup")
{
}

CopyFsBackupJob::CopyFsBackupJob(const CopyFsBackupJob &other)
    : AbstractBackupJob(other)
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

void CopyFsBackupJob::RunRepositoryBackup(const string &source,
                                          const string &destination,
                                          ResultCollection &results)
{
    //return pair<JobStatus*, FileBackupReport*>(NULL, NULL);
}

JobStatus *CopyFsBackupJob::CreateGlobalStatus(const AbstractBackupJob::ResultCollection &results)
{
    return new JobStatus(JobStatus::ERROR, "Still not implemented");
}

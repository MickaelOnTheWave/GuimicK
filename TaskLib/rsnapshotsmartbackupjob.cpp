#include "rsnapshotsmartbackupjob.h"

#include "rsnapshotconfigurationbuilder.h"
#include "rsnapshotrawbackupjob.h"

#include <cstdio>

using namespace std;

RsnapshotSmartBackupJob::RsnapshotSmartBackupJob()
    : AbstractBackupJob(),
      templateConfigurationFile(""), temporaryFile("")
{
}

RsnapshotSmartBackupJob::RsnapshotSmartBackupJob(const RsnapshotSmartBackupJob &other)
    : AbstractBackupJob(other),
      templateConfigurationFile(other.templateConfigurationFile),
      temporaryFile(other.temporaryFile),
      dataToBackup(other.dataToBackup)
{
}

RsnapshotSmartBackupJob::~RsnapshotSmartBackupJob()
{
}

string RsnapshotSmartBackupJob::GetName()
{
    return string("Rsnapshot Backup");
}

AbstractJob *RsnapshotSmartBackupJob::Clone()
{
    return new RsnapshotSmartBackupJob(*this);
}

JobStatus *RsnapshotSmartBackupJob::Run()
{
    JobStatus* status = AbstractBackupJob::Run();
    delete status;

    return RunConfiguredBackupJob();
}

void RsnapshotSmartBackupJob::SetTemplateConfigurationFile(const string &value)
{
    templateConfigurationFile = value;
}

void RsnapshotSmartBackupJob::SetTemporaryFile(const string &value)
{
    temporaryFile = value;
}

void RsnapshotSmartBackupJob::RunRepositoryBackup(const string& source,
                                                  const string& destination,
                                                  AbstractBackupJob::ResultCollection &)
{
    dataToBackup.push_back(make_pair(source, destination));
}

JobStatus *RsnapshotSmartBackupJob::RunConfiguredBackupJob()
{
    RsnapshotConfigurationBuilder builder(templateConfigurationFile);
    builder.SetRepository(repository);
    if (temporaryFile != "")
        builder.SetGeneratedConfigurationFile(temporaryFile);
    string configuration = builder.CreateConfigurationFile(dataToBackup);
    RsnapshotRawBackupJob* rawBackupJob = new RsnapshotRawBackupJob(repository, configuration);
    rawBackupJob->SetParentDebugManager(debugManager);

    JobStatus* status = rawBackupJob->Run();

    delete rawBackupJob;
    RemoveFile(configuration);
    return status;
}

void RsnapshotSmartBackupJob::RemoveFile(const string &file)
{
    remove(file.c_str());
}




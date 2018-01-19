#include "rsnapshotsmartbackupjob.h"

#include "rawcopyfsbackupjob.h"
#include "rsnapshotconfigurationbuilder.h"
#include "rsnapshotrawbackupjob.h"

#include <cstdio>

using namespace std;

static const string defaultName = "Rsnapshot Backup";

RsnapshotSmartBackupJob::RsnapshotSmartBackupJob()
    : AbstractBackupJob(defaultName),
      templateConfigurationFile(""), temporaryFile(""),
      waitBeforeRun(false), maxBackupCount(100)
{
}

RsnapshotSmartBackupJob::RsnapshotSmartBackupJob(const RsnapshotSmartBackupJob &other)
    : AbstractBackupJob(other),
      templateConfigurationFile(other.templateConfigurationFile),
      temporaryFile(other.temporaryFile),
      dataToBackup(other.dataToBackup),
      waitBeforeRun(other.waitBeforeRun),
      maxBackupCount(other.maxBackupCount)
{
}

RsnapshotSmartBackupJob::~RsnapshotSmartBackupJob()
{
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

string RsnapshotSmartBackupJob::GetTypeName() const
{
   return defaultName;
}

string RsnapshotSmartBackupJob::GetTemplateConfigurationFile() const
{
    return templateConfigurationFile;
}

void RsnapshotSmartBackupJob::SetTemplateConfigurationFile(const string &value)
{
    templateConfigurationFile = value;
}

string RsnapshotSmartBackupJob::GetTemporaryFile() const
{
    return temporaryFile;
}

void RsnapshotSmartBackupJob::SetTemporaryFile(const string &value)
{
    temporaryFile = value;
}

bool RsnapshotSmartBackupJob::GetWaitBeforeRun() const
{
    return waitBeforeRun;
}

void RsnapshotSmartBackupJob::SetWaitBeforeRun(const bool value)
{
    waitBeforeRun = value;
}

int RsnapshotSmartBackupJob::GetMaxBackupCount() const
{
    return maxBackupCount;
}

void RsnapshotSmartBackupJob::SetMaxBackupCount(const int value)
{
    maxBackupCount = value;
}

void RsnapshotSmartBackupJob::RunRepositoryBackup(const string& source,
                                                  const string& destination,
                                                  AbstractBackupJob::ResultCollection &)
{
   dataToBackup.push_back(make_pair(source, destination));
}

JobStatus* RsnapshotSmartBackupJob::RestoreBackup(const string& source, const string& destination)
{
   const int backupIndex = 0;

   stringstream weeklyString;
   weeklyString << "/weekly." << backupIndex;
   const string fullBackupSource = repository + weeklyString.str() + "/" + source;

   return RunRawCopy(fullBackupSource, destination);
}

JobStatus *RsnapshotSmartBackupJob::RunConfiguredBackupJob()
{
    RsnapshotConfigurationBuilder builder(templateConfigurationFile);
    builder.SetRepository(repository);
    if (temporaryFile != "")
        builder.SetGeneratedConfigurationFile(temporaryFile);
    string configuration = builder.CreateConfigurationFile(dataToBackup, maxBackupCount);
    RsnapshotRawBackupJob* rawBackupJob = new RsnapshotRawBackupJob(repository, configuration);
    rawBackupJob->SetParentDebugManager(debugManager);
    rawBackupJob->SetWaitBeforeRun(waitBeforeRun);
    rawBackupJob->SetName(GetName());

    JobStatus* status = rawBackupJob->Run();

    delete rawBackupJob;
    RemoveFile(configuration);
    return status;
}

void RsnapshotSmartBackupJob::RemoveFile(const string &file)
{
   remove(file.c_str());
}

JobStatus* RsnapshotSmartBackupJob::RunRawCopy(const string& source, const string& destination)
{
   RawCopyFsBackupJob rawCopyJob;
   rawCopyJob.CopyTarget(*this);
   rawCopyJob.AddFolder(source, destination);
   JobStatus* status = rawCopyJob.Run();

   if (status->IsOk())
      return new JobStatus(JobStatus::OK);
   else
      return new JobStatus(JobStatus::ERROR, "Raw Copy failed");
}




#include "rsnapshotsmartbackupjob.h"

#include <cstdio>

#include "filetools.h"
#include "rawcopyfsbackupjob.h"
#include "rsnapshotconfigurationbuilder.h"

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

void RsnapshotSmartBackupJob::SetRepository(const string& value)
{
   AbstractBackupJob::SetRepository(BuildFinalPath(value));
}

void RsnapshotSmartBackupJob::AddFolder(const string& source, const string& destination)
{
   AbstractBackupJob::AddFolder(BuildFinalPath(source), destination);
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

void RsnapshotSmartBackupJob::RunRepositoryBackup(const string&,
                                                  const string&,
                                                  AbstractBackupJob::ResultCollection &)
{
}

JobStatus* RsnapshotSmartBackupJob::RestoreBackup(const string& source, const string& destination)
{
   if (!FileTools::IsFolderEmpty(source))
      return RunRawCopy(source, destination);
   else
      return new JobStatus(JobStatus::OK);
}

string RsnapshotSmartBackupJob::CreateBackupSourcePath(const string& backupTag) const
{
   const int lastBackupIndex = 0;
   stringstream weeklyString;
   weeklyString << "weekly." << lastBackupIndex;
   return repository + weeklyString.str() + "/" + backupTag;
}

JobStatus *RsnapshotSmartBackupJob::RunConfiguredBackupJob()
{
   string configuration = CreateConfiguration();
   RsnapshotRawBackupJob* rawBackupJob = CreateRawJob(configuration);

   JobStatus* status = rawBackupJob->Run();

   FileTools::RemoveFile(configuration);
   delete rawBackupJob;
   return status;
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

string RsnapshotSmartBackupJob::AppendTrailingSlash(const string value) const
{
   const char slash = '/';
   string fixedValue = value;
   if (value[value.size()-1] != slash)
      fixedValue.push_back(slash);
   return fixedValue;
}

string RsnapshotSmartBackupJob::CreateConfiguration() const
{
   RsnapshotConfigurationBuilder builder(templateConfigurationFile);
   builder.SetRepository(repository);
   if (temporaryFile != "")
       builder.SetGeneratedConfigurationFile(temporaryFile);
   return builder.CreateConfigurationFile(folderList, maxBackupCount);
}

RsnapshotRawBackupJob* RsnapshotSmartBackupJob::CreateRawJob(const string& configuration) const
{
   RsnapshotRawBackupJob* rawBackupJob = new RsnapshotRawBackupJob(repository, configuration);
   rawBackupJob->SetParentDebugManager(debugManager);
   rawBackupJob->SetWaitBeforeRun(waitBeforeRun);
   rawBackupJob->SetName(GetName());
   return rawBackupJob;
}

string RsnapshotSmartBackupJob::BuildFinalPath(const string& inputPath) const
{
   const bool shouldBuildPath = (isTargetLocal && inputPath[0] != '/');
   return (shouldBuildPath) ? FileTools::BuildFullPath(inputPath) : inputPath;
}




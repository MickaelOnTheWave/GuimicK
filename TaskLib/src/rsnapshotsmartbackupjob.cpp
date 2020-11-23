#include "rsnapshotsmartbackupjob.h"

#include <cstdio>

#include "filetools.h"
#include "linuxcopyfsbackupjob.h"
#include "pathtools.h"
#include "rsnapshotconfigurationbuilder.h"

using namespace std;

static const wstring defaultName = L"Rsnapshot Backup";

RsnapshotSmartBackupJob::RsnapshotSmartBackupJob()
    : AbstractBackupJob(defaultName),
      templateConfigurationFile(L""), temporaryFile(L""),
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

wstring RsnapshotSmartBackupJob::GetTypeName() const
{
   return defaultName;
}

void RsnapshotSmartBackupJob::SetRepository(const wstring& value)
{
   AbstractBackupJob::SetRepository(BuildFinalPath(value));
}

wstring RsnapshotSmartBackupJob::GetTemplateConfigurationFile() const
{
    return templateConfigurationFile;
}

void RsnapshotSmartBackupJob::SetTemplateConfigurationFile(const wstring &value)
{
    templateConfigurationFile = value;
}

wstring RsnapshotSmartBackupJob::GetTemporaryFile() const
{
    return temporaryFile;
}

void RsnapshotSmartBackupJob::SetTemporaryFile(const wstring &value)
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

void RsnapshotSmartBackupJob::RunRepositoryBackup(const wstring&,
                                                  const wstring&,
                                                  AbstractBackupJob::ResultCollection &)
{
}

JobStatus* RsnapshotSmartBackupJob::RestoreBackupFromServer(const wstring& source, const wstring& destination)
{
   if (!FileTools::IsFolderEmpty(source))
      return RunRawCopy(source, destination);
   else
      return new JobStatus(JobStatus::Ok, L"");
}

JobStatus* RsnapshotSmartBackupJob::RestoreBackupFromClient(
      const BackupRestoreParameters& parameters,
      const BackupRestoreTarget& restoreTarget)
{
   wstring source = CreateBackupSourcePath(L"");

   LinuxCopyFsBackupJob copyJob;
   copyJob.SetTargetRemote(restoreTarget.user, restoreTarget.host);
   copyJob.AddFolder(source, parameters.destination);
   copyJob.SetOutputDebugInformation(DebugOutput::ON_ANY_ERROR);
   JobStatus* copyStatus = copyJob.Run();
   return copyStatus;
}

wstring RsnapshotSmartBackupJob::CreateBackupSourcePath(const wstring& backupTag) const
{
   const int lastBackupIndex = 0;
   wstringstream weeklywstring;
   weeklywstring << L"weekly." << lastBackupIndex;
   return repository + weeklywstring.str() + L"/" + backupTag;
}

JobStatus *RsnapshotSmartBackupJob::RunConfiguredBackupJob()
{
   debugManager->AddDataLine<bool>(L"Running - IsTargetLocal", target.isLocal);
   const wstring configuration = CreateConfiguration();
   debugManager->AddDataLine<wstring>(L"Rsnapshot configuration file", configuration);
   RsnapshotRawBackupJob* rawBackupJob = CreateRawJob(configuration);

   JobStatus* status = rawBackupJob->Run();

   if (temporaryFile == L"")
      FileTools::RemoveFile(configuration);

   delete rawBackupJob;
   return status;
}

JobStatus* RsnapshotSmartBackupJob::RunRawCopy(const wstring& source, const wstring& destination)
{
   LinuxCopyFsBackupJob rawCopyJob;
   rawCopyJob.CopyTarget(*this);
   rawCopyJob.AddFolder(source, destination);
   JobStatus* status = rawCopyJob.Run();

   if (status->IsOk())
      return new JobStatus(JobStatus::Ok);
   else
      return new JobStatus(JobStatus::Error, L"Raw Copy failed");
}

wstring RsnapshotSmartBackupJob::AppendTrailingSlash(const wstring value) const
{
   const char slash = '/';
   wstring fixedValue = value;
   if (value[value.size()-1] != slash)
      fixedValue.push_back(slash);
   return fixedValue;
}

wstring RsnapshotSmartBackupJob::CreateConfiguration() const
{
   RsnapshotConfigurationBuilder builder(templateConfigurationFile, debugManager);
   builder.SetRepository(repository);
   if (temporaryFile != L"")
       builder.SetGeneratedConfigurationFile(temporaryFile);
   return builder.CreateConfigurationFile(CreateRsnapshotBackupList(), maxBackupCount);
}

RsnapshotRawBackupJob* RsnapshotSmartBackupJob::CreateRawJob(const wstring& configuration) const
{
   RsnapshotRawBackupJob* rawBackupJob = new RsnapshotRawBackupJob(repository, configuration);
   rawBackupJob->SetParentDebugManager(debugManager);
   rawBackupJob->SetWaitBeforeRun(waitBeforeRun);
   rawBackupJob->SetName(GetName());
   return rawBackupJob;
}

wstring RsnapshotSmartBackupJob::BuildFinalPath(const wstring& inputPath) const
{
   debugManager->AddDataLine<wstring>(L"BuildFinalPath", inputPath);
   const bool shouldBuildPath = (target.isLocal && !PathTools::IsAbsolutePath(inputPath));
   debugManager->AddDataLine<bool>(L"WillBuildAbsolutePath", shouldBuildPath);
   return (shouldBuildPath) ? PathTools::BuildFullPath(inputPath) : inputPath;
}

AbstractBackupJob::BackupCollection RsnapshotSmartBackupJob::CreateRsnapshotBackupList() const
{
   BackupCollection finalBackupList;
   BackupCollection::const_iterator it = folderList.begin();
   for (; it!=folderList.end(); ++it)
   {
      const pair<wstring,wstring> newEntry(BuildFinalPath(it->first), it->second);
      finalBackupList.push_back(newEntry);
   }
   return finalBackupList;
}




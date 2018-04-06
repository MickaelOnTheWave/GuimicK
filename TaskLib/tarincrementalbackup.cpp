#include "tarincrementalbackup.h"

#include <sstream>

#include "consolejob.h"
#include "filetools.h"
#include "tartools.h"

using namespace std;

static const string defaultName = "Tar Incremental Backup";

TarIncrementalBackup::TarIncrementalBackup()
   : AbstractBackupJob(defaultName),
     maxIncrementsPerFullBackup(20), maxFullBackups(2)
{
}

TarIncrementalBackup::TarIncrementalBackup(const TarIncrementalBackup& other)
   : AbstractBackupJob(other),
     maxIncrementsPerFullBackup(other.maxIncrementsPerFullBackup),
     maxFullBackups(other.maxFullBackups)
{
}

AbstractJob* TarIncrementalBackup::Clone()
{
   return new TarIncrementalBackup(*this);
}

std::string TarIncrementalBackup::GetTypeName() const
{
   return defaultName;
}

JobStatus* TarIncrementalBackup::RestoreBackupFromServer(
      const string& destination,
      const int folderIndex,
      const int timeIndex)
{
   if (static_cast<unsigned int>(folderIndex) < folderList.size())
   {
      const string backupSource = CreateBackupSourcePath(folderList[folderIndex].second);
      const int lastArchiveIndex = FindArchiveLastBackupIndex(backupSource);
      const string archiveName = CreateIncrementalArchiveName(backupSource, timeIndex, lastArchiveIndex);

      TarTools tarTool(&target, debugManager);
      const int archiveIndex = lastArchiveIndex - timeIndex;
      const bool result = tarTool.ExtractIncrementalArchive(backupSource, archiveIndex, destination);
      if (result)
         return new JobStatus(JobStatus::OK, "");
      else
         return new JobStatus(JobStatus::ERROR, "Error while extracting");
   }
   else
      return new JobStatus(JobStatus::ERROR, "Invalid Repository Index");
}

int TarIncrementalBackup::GetMaxIncrementsCount() const
{
   return maxIncrementsPerFullBackup;
}

void TarIncrementalBackup::SetMaxIncrementsCount(const int value)
{
   maxIncrementsPerFullBackup = value;
}

int TarIncrementalBackup::GetMaxFullsCount() const
{
   return maxFullBackups;
}

void TarIncrementalBackup::SetMaxFullsCount(const int value)
{
   maxFullBackups = value;
}

void TarIncrementalBackup::RunRepositoryBackup(
      const string& source, const string& destination,
      AbstractBackupJob::ResultCollection& results)
{
   CreateNewBackup(source, destination, results);
   RemoveObsoleteBackups();
}

JobStatus* TarIncrementalBackup::RestoreBackupFromServer(const string& source,
                                                         const string& destination)
{
   TarTools tarTool(&target, debugManager);
   const bool result = tarTool.ExtractArchive(source, destination);
   if (result)
      return new JobStatus(JobStatus::OK, "");
   else
      return new JobStatus(JobStatus::ERROR, "Error while extracting");
}

void TarIncrementalBackup::CreateNewBackup(
      const string& source, const string& destination,
      AbstractBackupJob::ResultCollection& results)
{
   const string finalDestination = repository + destination;
   if (!DoesFullBackupExist(finalDestination))
      RunFullBackup(source, finalDestination, results);
   else
      RunIncrementalBackup(source, finalDestination, results);
}

void TarIncrementalBackup::RemoveObsoleteBackups()
{
   // TODO : implement removing obsolete backups
}

void TarIncrementalBackup::RunFullBackup(
      const std::string& source, const std::string& destination,
      AbstractBackupJob::ResultCollection& results)
{

   stringstream params;
   params << "cpvfh " << destination << " -g ";
   params << GetIndexFile(destination) << " -C " << source << " .";

   TarTools tarTool(&target, debugManager);
   tarTool.CreateArchive(params.str(), results);
}

void TarIncrementalBackup::RunIncrementalBackup(
      const std::string& source, const std::string& destination,
      AbstractBackupJob::ResultCollection& results)
{
   stringstream params;
   params << "cpvfh " << CreateIndexedDestination(destination) << " -g ";
   params << GetIndexFile(destination) << " -C " << source << " .";

   TarTools tarTool(&target, debugManager);
   tarTool.CreateIncrementalArchive(params.str(),
                                    CreateIndexedDestination(destination),
                                    destination, results);
}

bool TarIncrementalBackup::DoesFullBackupExist(const string& destination) const
{
   const bool archiveExists = FileTools::FileExists(destination);
   const bool indexExists = FileTools::FileExists(GetIndexFile(destination));
   return archiveExists && indexExists;
}

string TarIncrementalBackup::GetIndexFile(const string& destination) const
{
   return destination + ".snar";
}

string TarIncrementalBackup::CreateIndexedDestination(const string& destination) const
{
   int index = 0;
   string currentDestination;
   do
   {
      stringstream stream;
      stream << destination << "." << index;
      currentDestination = stream.str();
      ++index;
   }
   while (FileTools::FileExists(currentDestination));
   return currentDestination;
}

unsigned int TarIncrementalBackup::FindArchiveLastBackupIndex(const string& backupArchive) const
{
   int index = -1;
   while (true)
   {
      const string currentArchiveName = CreateArchiveName(backupArchive, index+1);
      if (FileTools::FileExists(currentArchiveName))
         ++index;
      else
         break;
   }

   return index;
}

string TarIncrementalBackup::CreateIncrementalArchiveName(const string& backupArchive,
                                                          const int timeIndex,
                                                          const int lastArchiveIndex) const
{
   const int archiveIndex = lastArchiveIndex - timeIndex;
   if (archiveIndex < 0)
      return backupArchive;
   else
      return CreateArchiveName(backupArchive, archiveIndex);
}

string TarIncrementalBackup::CreateArchiveName(const string& baseArchive,
                                               const int archiveIndex) const
{
   stringstream archive;
   archive << baseArchive << "." << archiveIndex;
   return archive.str();
}


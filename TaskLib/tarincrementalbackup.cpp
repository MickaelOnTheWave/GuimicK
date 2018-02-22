#include "tarincrementalbackup.h"

#include <sstream>

#include "consolejob.h"
#include "tartools.h"

using namespace std;

static const string defaultName = "Tar Incremental Backup";
static const string indexExtension = ".snar";

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
   // TODO : implement
   return new JobStatus(JobStatus::ERROR, "Restore not Implemented");
}

void TarIncrementalBackup::CreateNewBackup(
      const string& source, const string& destination,
      AbstractBackupJob::ResultCollection& results)
{
   if (!DoesFullBackupExist(destination))
      RunFullBackup(source, destination, results);
   else
      RunIncrementalBackup(source, destination, results);
}

void TarIncrementalBackup::RemoveObsoleteBackups()
{

}

void TarIncrementalBackup::RunFullBackup(
      const std::string& source, const std::string& destination,
      AbstractBackupJob::ResultCollection& results)
{
   const string indexFile = destination + indexExtension;

   stringstream params;
   params << "-cpzvf " << destination << " -g ";
   params << indexFile << " " << source;

   TarTools tarTool(debugManager, &target);
   tarTool.CreateArchive(params.str(), results);
}

void TarIncrementalBackup::RunIncrementalBackup(
      const std::string& source, const std::string& destination,
      AbstractBackupJob::ResultCollection& results)
{
   const string indexFile = destination + indexExtension;

   stringstream params;
   params << "-cpf " << destination << " -g ";
   params << indexFile << " " << source;

   ConsoleJob commandJob("tar", params.str());
   commandJob.RunWithoutStatus();

   // TODO : implement
   //tar -cpf backupfile.X.tar -g backupfile.snar folder/
}

bool TarIncrementalBackup::DoesFullBackupExist(const string& destination) const
{
   // TODO : implement
   return false;
}

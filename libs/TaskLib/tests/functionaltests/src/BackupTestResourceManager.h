#ifndef BACKUPTESTRESOURCEMANAGER_H
#define BACKUPTESTRESOURCEMANAGER_H

#include "abstractbackupjob.h"

#include <random>

class BackupTestResourceManager
{
public:
   BackupTestResourceManager();

   void Initialize(AbstractBackupJob* job);

   void Clean();

   void CheckResourcesConsistency() const;

   std::vector<std::string> CreateTestFiles(const int fileCount);
   std::vector<std::string> RemoveTestFiles(const int fileCount);
   std::vector<std::string> ModifyTestFiles(const int fileCount);

private:
   std::vector<std::string> GetSortedExistingFiles(const std::string& folder) const;
   std::vector<std::string> GetShuffledSourceFiles() const;
   std::vector<std::string> GetExistingFiles(const std::string& folder) const;

   std::string CreateRandomFilename() const;
   size_t SelectRandomFilesize() const;
   void ModifyFile(const std::string& filename);

   std::string backupSource;
   std::string backupDestination;
   mutable std::mt19937 randomGenerator;
};

#endif // BACKUPTESTRESOURCEMANAGER_H

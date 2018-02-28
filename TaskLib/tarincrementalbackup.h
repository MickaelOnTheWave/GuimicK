#ifndef TARINCREMENTALBACKUP_H
#define TARINCREMENTALBACKUP_H

#include "abstractbackupjob.h"

class TarIncrementalBackup : public AbstractBackupJob
{
public:
   TarIncrementalBackup();
   TarIncrementalBackup(const TarIncrementalBackup& other);

   virtual AbstractJob* Clone();

   virtual std::string GetTypeName() const;

   int GetMaxIncrementsCount() const;
   void SetMaxIncrementsCount(const int value);

   int GetMaxFullsCount() const;
   void SetMaxFullsCount(const int value);

protected:
    virtual void RunRepositoryBackup(const std::string& source,
                                     const std::string& destination,
                                     ResultCollection& results);

private:
   virtual JobStatus* RestoreBackupFromServer(const std::string& source,
                                              const std::string& destination);

   void CreateNewBackup(const std::string& source,
                        const std::string& destination,
                        ResultCollection& results);
   void RemoveObsoleteBackups();
   void RunFullBackup(const std::string& source,
                      const std::string& destination,
                      ResultCollection& results);
   void RunIncrementalBackup(const std::string& source,
                             const std::string& destination,
                             ResultCollection& results);

   bool DoesFullBackupExist(const std::string& destination) const;

   std::string GetIndexFile(const std::string& destination) const;
   std::string CreateIndexedDestination(const std::string& destination) const;

   int maxIncrementsPerFullBackup;
   int maxFullBackups;

};

#endif // TARINCREMENTALBACKUP_H

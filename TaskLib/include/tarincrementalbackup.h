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

   virtual JobStatus* RestoreBackupFromServer(const std::string& destination,
                                              const int folderIndex,
                                              const int timeIndex);

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
   void RunIncrementalBackup(const std::wstring& source,
                             const std::wstring& destination,
                             ResultCollection& results);

   bool DoesFullBackupExist(const std::wstring& destination) const;

   std::wstring GetIndexFile(const std::wstring& destination) const;
   std::wstring CreateIndexedDestination(const std::wstring& destination) const;

   unsigned int FindArchiveLastBackupIndex(const std::wstring& backupArchive) const;
   std::wstring CreateIncrementalArchiveName(const std::wstring& backupArchive,
                                             const int timeIndex,
                                             const int lastArchiveIndex) const;
   std::wstring CreateArchiveName(const std::wstring& baseArchive,
                                  const int archiveIndex) const;

   int maxIncrementsPerFullBackup;
   int maxFullBackups;

};

#endif // TARINCREMENTALBACKUP_H

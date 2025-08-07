#ifndef BACKUPTESTSFIXTURE_H
#define BACKUPTESTSFIXTURE_H

#include "abstractbackupjob.h"
#include "BackupTestResourceManager.h"

class BackupTestsFixture
{
public:
   virtual ~BackupTestsFixture();

   void RunBackupTests(AbstractBackupJob* _backupJob);

private:
   void RunBackupStep(const std::string& sectionTitle, const JobStatus& expectedStatus);

   JobStatus CreateAddedStatus(const std::vector<std::string>& filenames) const;
   JobStatus CreateRemovedStatus(const std::vector<std::string>& filenames) const;
   JobStatus CreateModifiedStatus(const std::vector<std::string>& filenames) const;

   AbstractBackupJob* backupJob = nullptr;
   BackupTestResourceManager resourceManager;
};

#endif // BACKUPTESTSFIXTURE_H

/*
 * TaskLib
 * Copyright (C) 2025 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <catch2/catch.hpp>

#include "abstractbackupjob.h"
#include "rsnapshotsmartbackupjob.h"
#include "rsynccopyfsbackupjob.h"
#include "tarbackupjob.h"

/*******************/

class BackupTestResourceManager
{
public:
   BackupTestResourceManager(const int same, const int added, const int removed, const int modified)
   {
   }

   void Initialize()
   {
      PopulateSource();
      PopulateDestination();
   }

   void Clean()
   {
      CleanRepository();
      CleanSource();
      CleanDestination();
   }

   void CheckResourcesConsistency()
   {

   }

private:

};

class BackupTestsFixture
{
public:

   void RunBackupTests(AbstractBackupJob* backupJob)
    {
      {
         BackupTestResourceManager resourceManager(2, 5, 0, 0);
         RunBackupTest("Added 5 files", backupJob, resourceManager);
      }

      {
         BackupTestResourceManager resourceManager(2, 0, 5, 0);
         RunBackupTest("Removed 5 files", backupJob, resourceManager);
      }

      {
         BackupTestResourceManager resourceManager(2, 0, 0, 5);
         RunBackupTest("Modified 5 files", backupJob, resourceManager);
      }
    }

private:
    void RunBackupTest(const std::string& sectionTitle, AbstractBackupJob* backupJob, BackupTestResourceManager& testResourceManager)
    {
       SECTION(sectionTitle)
       {
          testResourceManager.Initialize();

          backupJob->SetRepository(L"repository");
          backupJob->AddFolder(L"backupSource", L"backupDestination");

          JobStatus* status = backupJob->Run();

          REQUIRE(*status == expectedStatus);
          testResourceManager.CheckResourcesConsistency();

          delete status;
          delete backupJob;

          testResourceManager.Clean();
       }
    }
};


/*******************/

TEST_CASE_METHOD(BackupTestsFixture, "TarBackupJob")
{
   RunBackupTests(new TarBackupJob());
}

TEST_CASE_METHOD(BackupTestsFixture, "RsyncCopyFsBackupJob")
{
   RunBackupTests(new RsyncCopyFsBackupJob());
}

TEST_CASE_METHOD(BackupTestsFixture, "RsnapshotSmartBackupJob")
{
   RunBackupTests(new RsnapshotSmartBackupJob());
}



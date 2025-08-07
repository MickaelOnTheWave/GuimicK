#include "BackupTestsFixture.h"

#include <catch2/catch.hpp>

#include "stringtools.h"

BackupTestsFixture::~BackupTestsFixture()
{
   delete backupJob;
   resourceManager.Clean();
}

void BackupTestsFixture::RunBackupTests(AbstractBackupJob* _backupJob)
{
   backupJob = _backupJob;
   // 1. Initialize backup job and initial data
   backupJob->SetRepository(L"repository");
   backupJob->AddFolder(L"backupSource", L"backupDestination");

   {
      const std::vector<std::string> filenames = resourceManager.CreateTestFiles(5);
      const JobStatus expectedStatus = CreateAddedStatus(filenames);
      RunBackupStep("Added 5 files", expectedStatus);
   }

   {
      const std::vector<std::string> filenames = resourceManager.RemoveTestFiles(5);
      const JobStatus expectedStatus = CreateRemovedStatus(filenames);
      RunBackupStep("Removed 5 files", expectedStatus);
   }

   {
      const std::vector<std::string> filenames = resourceManager.ModifyTestFiles(5);
      const JobStatus expectedStatus = CreateModifiedStatus(filenames);
      RunBackupStep("Modified 5 files", expectedStatus);
   }
}

void BackupTestsFixture::RunBackupStep(const std::string& sectionTitle, const JobStatus& expectedStatus)
{
   backupJob->SetRepository(L"repository");
   backupJob->AddFolder(L"backupSource", L"backupDestination");

   JobStatus* status = backupJob->Run();

   REQUIRE(*status == expectedStatus);
   resourceManager.CheckResourcesConsistency();

   delete status;
   delete backupJob;
}

JobStatus BackupTestsFixture::CreateAddedStatus(const std::vector<std::string>& filenames) const
{
   JobStatus status(JobStatus::Ok);
   std::string description;
   description += std::to_string(filenames.size()) + " added.\n";
   for (const auto& filename : filenames)
      description += filename + "\n";
   status.SetDescription(StringTools::Utf8ToUnicode(description));
   return status;
}

JobStatus BackupTestsFixture::CreateRemovedStatus(const std::vector<std::string>& filenames) const
{
   JobStatus status(JobStatus::Ok);
   std::string description;
   description += std::to_string(filenames.size()) + " removed.\n";
   for (const auto& filename : filenames)
      description += filename + "\n";
   status.SetDescription(StringTools::Utf8ToUnicode(description));
   return status;
}

JobStatus BackupTestsFixture::CreateModifiedStatus(const std::vector<std::string>& filenames) const
{
   JobStatus status(JobStatus::Ok);
   std::string description;
   description += std::to_string(filenames.size()) + " modified.\n";
   for (const auto& filename : filenames)
      description += filename + "\n";
   status.SetDescription(StringTools::Utf8ToUnicode(description));
   return status;
}

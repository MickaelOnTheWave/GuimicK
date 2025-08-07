#include "BackupTestResourceManager.h"

#include <algorithm>
#include <catch2/catch.hpp>

#include "filetools.h"
#include "stringtools.h"

using namespace std;

BackupTestResourceManager::BackupTestResourceManager()
{
   std::random_device rd;
   randomGenerator = std::mt19937(rd());
}

void BackupTestResourceManager::Initialize(AbstractBackupJob* job)
{
   std::vector<std::pair<std::wstring, std::wstring>> folderList;
   job->GetFolderList(folderList);
   backupSource = StringTools::UnicodeToUtf8(folderList.front().first);
   backupDestination = StringTools::UnicodeToUtf8(folderList.front().second);
}

void BackupTestResourceManager::Clean()
{
   const bool keepRootFolder = false;
   FileTools::RemoveFolder(backupSource, keepRootFolder);
   FileTools::RemoveFolder(backupDestination, keepRootFolder);
}

void BackupTestResourceManager::CheckResourcesConsistency() const
{
   std::vector<std::string> allSourceFiles = GetSortedExistingFiles(backupSource);
   std::vector<std::string> allDestinationFiles = GetSortedExistingFiles(backupDestination);
   REQUIRE(allSourceFiles == allDestinationFiles);
}

std::vector<std::string> BackupTestResourceManager::CreateTestFiles(const int fileCount)
{
   std::vector<std::string> filenames;
   for (int i=0; i<fileCount; ++i)
   {
      const std::string filename = CreateRandomFilename();
      const size_t filesize = SelectRandomFilesize();
      FileTools::CreateDummyFile(filename, filesize, true);
      filenames.push_back(filename);
   }
   return filenames;
}

std::vector<std::string> BackupTestResourceManager::RemoveTestFiles(const int fileCount)
{
   std::vector<std::string> filenames;
   std::vector<std::string> allFiles = GetShuffledSourceFiles();
   for (int i=0; i<fileCount && !allFiles.empty(); ++i)
   {
      const std::string filename = allFiles.back();
      FileTools::RemoveFile(StringTools::Utf8ToUnicode(filename));
      filenames.push_back(filename);
      allFiles.pop_back();
   }
   return filenames;
}

std::vector<std::string> BackupTestResourceManager::ModifyTestFiles(const int fileCount)
{
   std::vector<std::string> filenames;
   std::vector<std::string> allFiles = GetShuffledSourceFiles();
   for (int i=0; i<fileCount && !allFiles.empty(); ++i)
   {
      const std::string filename = allFiles.back();
      ModifyFile(filename);
      filenames.push_back(filename);
      allFiles.pop_back();
   }
   return filenames;
}

vector<string> BackupTestResourceManager::GetSortedExistingFiles(const string& folder) const
{
   vector<string> existingFiles = GetExistingFiles(folder);
   std::sort(existingFiles.begin(), existingFiles.end());
   return existingFiles;
}

vector<string> BackupTestResourceManager::GetShuffledSourceFiles() const
{
   vector<string> existingFiles = GetExistingFiles(backupSource);
   std::shuffle(existingFiles.begin(), existingFiles.end(), randomGenerator);
   return existingFiles;
}

vector<string> BackupTestResourceManager::GetExistingFiles(const string& folder) const
{
   vector<string> existingFiles;
   return existingFiles;
}

string BackupTestResourceManager::CreateRandomFilename() const
{
   string filename("RandomFile-");
   for (int i=0; i<5; ++i)
   {
      std::uniform_int_distribution<int> dist(97, 122);
      filename += static_cast<char>(dist(randomGenerator));
   }
   filename += ".txt";
   return filename;
}

size_t BackupTestResourceManager::SelectRandomFilesize() const
{
   std::uniform_int_distribution<int> dist(10, 100);
   return dist(randomGenerator);
}

void BackupTestResourceManager::ModifyFile(const std::string &filename)
{

}

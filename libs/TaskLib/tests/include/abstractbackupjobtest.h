#ifndef ABSTRACTBACKUPJOBTEST_H
#define ABSTRACTBACKUPJOBTEST_H

#include <functional>

#include "abstractbackupjob.h"

class AbstractBackupJobTest
{
public:
   AbstractBackupJobTest(const std::wstring& dataPrefix,
                         const std::wstring& errorPrefix);
   virtual ~AbstractBackupJobTest() = default;

   void TestBackupAndRestore();

protected:   
   virtual AbstractBackupJob* CreateNewJob() = 0;

   using TestFunction = std::function<void(const std::wstring& sourceBefore,
                                           const std::wstring& sourceNow,
                                           const bool isRemote)>;
   void TestOnDataset(TestFunction testFunction);

   const std::wstring backupRepository = L"repository/";
   std::wstring currentSourceFolder = L"currentFolderToBackup";
   std::wstring currentTestCaseFolder = L"";
   std::wstring currentTestCaseName = L"";

private:
   void init();
   void cleanup();

   void RunBackup(AbstractBackupJob* job, const std::wstring& folder);
   void RunRestore(AbstractBackupJob* job, const std::wstring& folder);

   void PrepareBackup(AbstractBackupJob* job, const std::wstring& folder);

   virtual std::wstring GetBackupDestination() const;

   std::wstring dataFolderPrefix;
};

#endif // ABSTRACTBACKUPJOBTEST_H

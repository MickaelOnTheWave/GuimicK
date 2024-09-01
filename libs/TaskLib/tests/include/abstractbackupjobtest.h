#ifndef ABSTRACTBACKUPJOBTEST_H
#define ABSTRACTBACKUPJOBTEST_H

#include "abstractbackupjob.h"

class AbstractBackupJobTest
{
public:
   AbstractBackupJobTest(const std::wstring& dataPrefix,
                         const std::wstring& errorPrefix);
   virtual ~AbstractBackupJobTest() = default;

protected:   
   void LoadExternalDataSamples(const bool isRemote);

   virtual AbstractBackupJob* CreateNewJob() = 0;

   const std::wstring backupRepository = L"repository/";
   std::wstring currentSourceFolder = L"currentFolderToBackup";
   std::wstring currentTestCaseFolder = L"";
   std::wstring currentTestCaseName = L"";

private:
    void init();
    void cleanup();

    void testBackupAndRestore_data();
    void testBackupAndRestore();

    void RunBackup(AbstractBackupJob* job, const std::wstring& folder);
    void RunRestore(AbstractBackupJob* job, const std::wstring& folder);

    void PrepareBackup(AbstractBackupJob* job, const std::wstring& folder);

    virtual std::wstring GetBackupDestination() const;
};

#endif // ABSTRACTBACKUPJOBTEST_H

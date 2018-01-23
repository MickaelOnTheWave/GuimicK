#ifndef ABSTRACTBACKUPJOBTEST_H
#define ABSTRACTBACKUPJOBTEST_H

#include "qttestsuite.h"

#include "abstractbackupjob.h"

class AbstractBackupJobTest : public QtTestSuite
{
   Q_OBJECT
public:
   AbstractBackupJobTest(const std::string& dataPrefix,
                         const std::string& errorPrefix);
   virtual ~AbstractBackupJobTest() = default;

protected:
   void LoadExternalDataSamples(const bool isRemote);

   const std::string backupRepository = "repository/";
   const std::string currentSourceFolder = "currentFolderToBackup";
   std::string currentTestCaseFolder = "";
   std::string currentTestCaseName = "";

private Q_SLOTS:
    void init();
    void cleanup();

    void testBackupAndRestore_data();
    void testBackupAndRestore();

private:
    void RunBackup(AbstractBackupJob* job, const std::string& folder);
    void RunRestore(AbstractBackupJob* job, const std::string& folder);

    void PrepareBackup(AbstractBackupJob* job, const std::string& folder);

    virtual AbstractBackupJob* CreateNewJob() = 0;
};

#endif // ABSTRACTBACKUPJOBTEST_H

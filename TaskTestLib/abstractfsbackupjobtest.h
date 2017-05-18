#ifndef ABSTRACTFSBACKUPJOBTEST_H
#define ABSTRACTFSBACKUPJOBTEST_H

#include "qttestsuite.h"
#include "jobstatus.h"

class AbstractFsBackupJobTest : public QtTestSuite
{
    Q_OBJECT
public:
    AbstractFsBackupJobTest(const std::string& dataPrefix);
    virtual ~AbstractFsBackupJobTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testRunBackup_data();
    void testRunBackup();

protected:
    virtual void ProcessingBetweenBackups();
    virtual void CheckBackedUpDataIsOk() = 0;
    virtual JobStatus* RunBackupJob() = 0;

    void LoadExternalDataSamples(const bool isRemote);

    const std::string currentSourceFolder = "currentFolderToBackup";
    std::string currentTestCaseFolder = "";

private:
    JobStatus* RunBackupOnDataFolder(const std::string &folder);
    void CheckStatus(JobStatus* status);
    void CheckTextContent(const std::string& content, const QString& referenceFile);

    std::string currentTestCaseName = "";
};

#endif // ABSTRACTFSBACKUPJOBTEST_H

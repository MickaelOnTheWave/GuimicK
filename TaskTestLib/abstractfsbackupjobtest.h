#ifndef ABSTRACTFSBACKUPJOBTEST_H
#define ABSTRACTFSBACKUPJOBTEST_H

#include "abstractbackupjob.h"
#include "qttestsuite.h"
#include "jobstatus.h"

class AbstractFsBackupJobTest : public QtTestSuite
{
    Q_OBJECT
public:
    AbstractFsBackupJobTest(const std::string& dataPrefix,
                            const std::string& errorPrefix);
    virtual ~AbstractFsBackupJobTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testRunBackup_data();
    void testRunBackup();

    void testCreatesOnlyOneAttachment();
    void testCreatesDebugAttachment();

protected:
    virtual void ProcessingBetweenBackups();
    virtual void CheckBackedUpDataIsOk();
    virtual JobStatus* RunBackupJob(const bool isRemote,
                                    const bool useDebug);
    virtual AbstractBackupJob* CreateNewJob() = 0;

    void LoadExternalDataSamples(const bool isRemote);

    JobStatus *RunBackups(const std::string& folderBefore,
                          const std::string& folderNow,
                          const bool isRemote, const bool useDebug = false);

    const std::string currentSourceFolder = "currentFolderToBackup";
    std::string currentTestCaseFolder = "";

private:
    JobStatus* RunBackupOnDataFolder(const std::string &folder,
                                     const bool isRemote, const bool useDebug);
    void CheckStatus(JobStatus* status);
    void CheckTextContent(const std::string& content, const QString& referenceFile);

    void testCheckJobAttachments(const bool debugOutput,
                                 const std::vector<std::string>& expectedAttachments);

    JobStatus* RunBackupJob(AbstractBackupJob* job,
                            const bool isRemote, const bool useDebug);

    JobStatus* RunDummyBackup(const bool debugOutput);
    void CheckStatusAttachments(JobStatus* status,
                                const std::vector<std::string>& expectedAttachments);

    std::string GetJobAttachmentName();
    std::string GetJobDebugName();

    std::string currentTestCaseName = "";
};

#endif // ABSTRACTFSBACKUPJOBTEST_H

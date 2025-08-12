#ifndef ABSTRACTFSBACKUPJOBTEST_H
#define ABSTRACTFSBACKUPJOBTEST_H

#include "abstractbackupjobtest.h"
#include "jobstatus.h"

class AbstractFsBackupJobTest : public AbstractBackupJobTest
{
public:
   AbstractFsBackupJobTest(const std::wstring& dataPrefix,
                         const std::wstring& errorPrefix);
   virtual ~AbstractFsBackupJobTest() = default;

   void TestRunBackup();

   void TestCreatesOnlyOneAttachment();
   void TestCreatesDebugAttachment();

protected:
    virtual void ProcessingBetweenBackups();
    virtual void CheckBackedUpDataIsOk();
    virtual JobStatus* RunBackupJob(const bool isRemote,
                                    const bool useDebug);

    JobStatus *RunBackups(const std::wstring& folderBefore,
                          const std::wstring& folderNow,
                          const bool isRemote, const bool useDebug = false);

private:
    JobStatus* RunBackupOnDataFolder(const std::wstring &folder,
                                     const bool isRemote, const bool useDebug);
    void CheckStatus(JobStatus* status);
    void CheckTextContent(const std::wstring& content, const std::wstring& referenceFile);

    void testCheckJobAttachments(const bool debugOutput,
                                 const std::vector<std::wstring>& expectedAttachments);

    JobStatus* RunBackupJob(AbstractBackupJob* job,
                            const bool isRemote, const bool useDebug);

    JobStatus* RunDummyBackup(const bool debugOutput);
    void CheckStatusAttachments(JobStatus* status,
                                const std::vector<std::wstring>& expectedAttachments);

    std::wstring GetJobAttachmentName();
    std::wstring GetJobDebugName();
};

#endif // ABSTRACTFSBACKUPJOBTEST_H

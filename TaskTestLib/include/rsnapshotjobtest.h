#ifndef RNAPSHOTJOBTEST_H
#define RNAPSHOTJOBTEST_H

#include "abstractfsbackupjobtest.h"

#include <string>

#include "jobstatus.h"

class RsnapshotJobTest : public AbstractFsBackupJobTest
{
    Q_OBJECT

public:
    RsnapshotJobTest(const std::string& dataPrefix,
                     const std::string& errorPrefix);
    virtual ~RsnapshotJobTest();

private Q_SLOTS:

    void testCreate_InvalidSource();

    void testRunBackup_data();

    void testSmartCreator_TempFileIsCleaned();
    void testSmartCreator_TempFileDoesNotOverwrite();

    // TODO : test maxBackups property. Check backup recycling and
    // that the property is respected.

protected:
    virtual void ProcessingBetweenBackups();
    virtual void CheckBackedUpDataIsOk();
    virtual AbstractBackupJob* CreateNewJob();

private:
    JobStatus* RunRsnapshotJob(const std::string& tempConfigurationFile = "");

    std::string GetRsnapshotBackupFolder(const int number) const;

    unsigned int GetFileNumberInCurrentFolder();
};

#endif // RNAPSHOTJOBTEST_H

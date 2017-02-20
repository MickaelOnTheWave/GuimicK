#ifndef RNAPSHOTJOBTEST_H
#define RNAPSHOTJOBTEST_H

#include <QObject>

#include <string>

#include "jobstatus.h"

class RsnapshotJobTest : public QObject
{
    Q_OBJECT

public:
    RsnapshotJobTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testCreate_InvalidSource();

    void testRunBackup_data();
    void testRunBackup();

private:
    JobStatus* RunBackupOnDataFolder(const std::string &folder);
    JobStatus* RunJob();
    void CheckStatus(JobStatus* status);
    void CheckFiles();

    void CreateConfigurationFile(const QString& folder);
    JobStatus* RunRsnapshotJob();
    std::string BuildFullPathOnCurrentDir(const std::string& name);

    void CheckTextContent(const std::string& content, const QString& referenceFile);
    void CheckFoldersHaveSameContent(const std::string& folder1, const std::string& folder2);
    std::string GetRsnapshotBackupFolder(const int number) const;

    std::string repository;
    std::string currentTestCaseName = "";
    std::string currentTestCaseFolder = "";
};

#endif // RNAPSHOTJOBTEST_H

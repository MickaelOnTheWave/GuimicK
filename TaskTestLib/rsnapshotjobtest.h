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
    JobStatus* RunBackupOnDataFolder(const QString& folder);
    JobStatus* RunJob();
    void CheckStatus(JobStatus* status);
    void CheckFiles();

    void CreateConfigurationFile(const QString& folder);
    JobStatus* RunRsnapshotJob();
    std::string BuildFullPathOnCurrentDir(const std::string& name);

    std::string repository;

};

#endif // RNAPSHOTJOBTEST_H

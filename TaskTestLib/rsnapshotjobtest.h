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
    void testCreate_AllValid();

    void testRunBackup_data();
    void testRunBackup();

/*    void testUpdate_data();
    void testUpdate();
    void testUpdate_MultipleRepositories();
    void testUpdate_InvalidSource();*/

private:
    void CreateDestinationFolder();
    void RemoveDestinationFolder();
    JobStatus* RunJob();
    void CheckStatus(JobStatus* status);
    void CheckFiles();

    void CreateConfigurationFile(const QString& folder);
    JobStatus* RunRsnapshotJob();
    std::string BuildFullPathOnCurrentDir(const std::string& name);

    std::string repository;

};

#endif // RNAPSHOTJOBTEST_H

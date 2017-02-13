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

/*    void testUpdate_data();
    void testUpdate();
    void testUpdate_MultipleRepositories();
    void testUpdate_InvalidSource();*/

private:
    void CreateConfigurationFile(const QString& folder);
    JobStatus* RunRsnapshotJob();
    void CheckStatusIsInvalid(JobStatus* status);
    std::string BuildFullPathOnCurrentDir(const std::string& name);

    std::string repository;

};

#endif // RNAPSHOTJOBTEST_H

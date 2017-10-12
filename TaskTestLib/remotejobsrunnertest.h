#ifndef REMOTEJOBSRUNNERTEST_H
#define REMOTEJOBSRUNNERTEST_H

#include "qttestsuite.h"

#include "client.h"
#include "workresultdata.h"

class RemoteJobsRunnerTest : public QtTestSuite
{
   Q_OBJECT

public:
   RemoteJobsRunnerTest();
   virtual ~RemoteJobsRunnerTest() = default;

private Q_SLOTS:
    void testRun();

private:
    Client* CreateInitializedClient();
    void InsertFilledRemoteJob(Client* client, const int remoteJobsCount);
    AbstractJob* CreateDummyJob(const std::string& tag, const int index);
    void CheckResultJobList(WorkResultData* result,
                            const unsigned long expectedCount);

};

#endif // REMOTEJOBSRUNNERTEST_H
